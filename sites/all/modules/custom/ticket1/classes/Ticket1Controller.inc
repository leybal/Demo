<?php

interface Ticket1ControllerInterface extends DrupalEntityControllerInterface {
  public function create(array $values = array());
  public function save($entity);
  public function delete($entity);
}

class Ticket1Controller extends EntityAPIController implements Ticket1ControllerInterface {

  /**
   * Create and return a new entity_example_basic entity.
   */
  public function create(array $values = array()) {
    global $user;

    // Adding defaults
    $values += array(
      'uid' => $user->uid,
      'seats' => 0,
      'created' => time(),
    );

    return parent::create($values);
  }


}
