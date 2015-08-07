<?php

interface Ticket1ControllerInterface extends DrupalEntityControllerInterface {
  public function create(array $values = array());
  public function save($entity);
  public function delete($entity);
}

class Ticket1Controller extends EntityAPIController implements Ticket1ControllerInterface {
  public function create(array $values = array()) {
    global $user;

    $values += array(
      'uid' => $user->uid,
      'seats' => 0,
      'created' => time(),
    );

    return parent::create($values);
  }

  /**
   * Saves the custom fields using drupal_write_record().
   */
  public function save($entity) {
    // If our entity has no basic_id, then we need to give it a
    // time of creation.
    if (empty($entity->basic_id)) {
      $entity->created = time();
    }
    // Invoke hook_entity_presave().
    module_invoke_all('entity_presave', $entity, 'entity_example_basic');
    // The 'primary_keys' argument determines whether this will be an insert
    // or an update. So if the entity already has an ID, we'll specify
    // basic_id as the key.
    $primary_keys = $entity->basic_id ? 'basic_id' : array();
    // Write out the entity record.
    drupal_write_record('entity_example_basic', $entity, $primary_keys);
    // We're going to invoke either hook_entity_update() or
    // hook_entity_insert(), depending on whether or not this is a
    // new entity. We'll just store the name of hook_entity_insert()
    // and change it if we need to.
    $invocation = 'entity_insert';
    // Now we need to either insert or update the fields which are
    // attached to this entity. We use the same primary_keys logic
    // to determine whether to update or insert, and which hook we
    // need to invoke.
    if (empty($primary_keys)) {
      field_attach_insert('entity_example_basic', $entity);
    }
    else {
      field_attach_update('entity_example_basic', $entity);
      $invocation = 'entity_update';
    }
    // Invoke either hook_entity_update() or hook_entity_insert().
    module_invoke_all($invocation, $entity, 'entity_example_basic');
    return $entity;
  }

  /**
   * Delete a single entity.
   *
   * Really a convenience function for deleteMultiple().
   */
  public function delete($entity) {
    $this->deleteMultiple(array($entity));
  }

  /**
   * Delete one or more entity_example_basic entities.
   *
   * Deletion is unfortunately not supported in the base
   * DrupalDefaultEntityController class.
   *
   * @param array $entities
   *   An array of entity IDs or a single numeric ID.
   */
  public function deleteMultiple($entities) {
    $basic_ids = array();
    if (!empty($entities)) {
      $transaction = db_transaction();
      try {
        foreach ($entities as $entity) {
          // Invoke hook_entity_delete().
          module_invoke_all('entity_delete', $entity, 'entity_example_basic');
          field_attach_delete('entity_example_basic', $entity);
          $basic_ids[] = $entity->basic_id;
        }
        db_delete('entity_example_basic')
          ->condition('basic_id', $basic_ids, 'IN')
          ->execute();
      }
      catch (Exception $e) {
        $transaction->rollback();
        watchdog_exception('entity_example', $e);
        throw $e;
      }
    }
  }
}