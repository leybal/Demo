<?php

/**
 * Simple filter to handle greater than/less than filters
 */
class interval_select_handler_filter_interval_surface extends views_handler_filter_numeric {
  var $always_multiple = TRUE;
  function option_definition() {
    $options = parent::option_definition();

    $options['value'] = array(
      'contains' => array(
        'value' => array('default' => ''),
      ),
    );

    return $options;
  }

  function operators() {
    $operators = array(
      'between' => array(
        'title' => t('Is between'),
        'method' => 'op_between',
        'short' => t('between'),
        'values' => 2,
      ),
    );

    return $operators;
  }

  /**
   * Provide a list of all the numeric operators
   */
  function operator_options($which = 'title') {
    $options = array();
    foreach ($this->operators() as $id => $info) {
      $options[$id] = $info[$which];
    }

    return $options;
  }

  function operator_values($values = 1) {
    $options = array();
    foreach ($this->operators() as $id => $info) {
      if ($info['values'] == $values) {
        $options[] = $id;
      }
    }

    return $options;
  }
  /**
   * Provide a simple textfield for equality
   */
  function value_form(&$form, &$form_state) {
    $form['value']['#tree'] = TRUE;

    // We have to make some choices when creating this as an exposed
    // filter form. For example, if the operator is locked and thus
    // not rendered, we can't render dependencies; instead we only
    // render the form items we need.
    $which = 'all';
    if (!empty($form['operator'])) {
      $source = ($form['operator']['#type'] == 'radios') ? 'radio:options[operator]' : 'edit-options-operator';
    }

    if (!empty($form_state['exposed'])) {
      $identifier = $this->options['expose']['identifier'];

      if (empty($this->options['expose']['use_operator']) || empty($this->options['expose']['operator_id'])) {
        // exposed and locked.
        $which = in_array($this->operator, $this->operator_values(2)) ? 'minmax' : 'value';
      }
      else {
        $source = 'edit-' . drupal_html_id($this->options['expose']['operator_id']);
      }
    }

    if ($which == 'all') {
      $form['value']['value'] = array(
        '#type' => 'textfield',
        '#title' => empty($form_state['exposed']) ? t('Value') : '',
        '#size' => 30,
        '#default_value' => $this->value['value'],
        '#dependency' => array($source => $this->operator_values(1)),
      );
      if (!empty($form_state['exposed']) && !isset($form_state['input'][$identifier]['value'])) {
        $form_state['input'][$identifier]['value'] = $this->value['value'];
      }
    }
    elseif ($which == 'value') {
      // When exposed we drop the value-value and just do value if
      // the operator is locked.
      $form['value'] = array(
        '#type' => 'textfield',
        '#title' => empty($form_state['exposed']) ? t('Value') : '',
        '#size' => 30,
        '#default_value' => $this->value['value'],
      );
      if (!empty($form_state['exposed']) && !isset($form_state['input'][$identifier])) {
        $form_state['input'][$identifier] = $this->value['value'];
      }
    }

    if ($which == 'all' || $which == 'minmax') {
        $tranches = array(
          1000000 => 'Indifférent',
          80 => '< 80 M2',
          100 => '80 à 100 M2',
          130 => '100 à 130 M2',
          160 => '130 à 160 M2',
          200 => '160 à 200 M2',
          250 => '200 à 250 M2',
          300 => '+ de 250 M2',
        );
      $form['value']['interval'] = array(
        '#type' => 'select',
        '#title' => '',
        '#options' => $tranches,
      );
      if ($which == 'all') {
        $dependency = array(
          '#dependency' => array($source => $this->operator_values(2)),
        );
      }

      if (!isset($form['value'])) {
        // Ensure there is something in the 'value'.
        $form['value'] = array(
          '#type' => 'value',
          '#value' => NULL
        );
      }
    }
  }

  function query() {
    $this->ensure_my_table();
    $field = "$this->table_alias.$this->real_field";

    $info = $this->operators();
    if (!empty($info[$this->operator]['method'])) {
      $this->{$info[$this->operator]['method']}($field);
    }
  }

  function op_between($field) {
        $min = 0;
        $max = 1000000;
        switch ($this->value['interval']) {
          case 80:
            $min = 0;
            $max = 80;
            break;
          case 100:
            $min = 80;
            $max = 100;
            break;
          case 130:
            $min = 100;
            $max = 130;
            break;
          case 160:
            $min = 130;
            $max = 160;
            break;
          case 200:
            $min = 160;
            $max = 200;
            break;
          case 250:
            $min = 200;
            $max = 250;
            break;        
          case 300:
            $min = 250;
            $max = 1000000;
            break;        
        }     
        if ($max ==  80) {  
          $this->query->add_where($this->options['group'], $field, $max, '<=');
        } elseif ($min <> 0 && $max <>  1000000) {  
          $this->query->add_where($this->options['group'], $field, array($min, $max), 'BETWEEN');
        }
  }

  function op_simple($field) {
    $this->query->add_where($this->options['group'], $field, $this->value['value'], $this->operator);
  }

  function op_empty($field) {
    if ($this->operator == 'empty') {
      $operator = "IS NULL";
    }
    else {
      $operator = "IS NOT NULL";
    }

    $this->query->add_where($this->options['group'], $field, NULL, $operator);
  }

  function admin_summary() {
    if (!empty($this->options['exposed'])) {
      return t('exposed');
    }

    $options = $this->operator_options('short');
    $output = check_plain($options[$this->operator]);
    if (in_array($this->operator, $this->operator_values(2))) {
      $output .= ' ' . t('@min and @max', array('@min' => $this->value['interval'], '@max' => $this->value['interval']));
    }
    elseif (in_array($this->operator, $this->operator_values(1))) {
      $output .= ' ' . check_plain($this->value['value']);
    }
    return $output;
  }

  /**
   * Do some minor translation of the exposed input
   */
  function accept_exposed_input($input) {
    if (empty($this->options['exposed'])) {
      return TRUE;
    }

    // rewrite the input value so that it's in the correct format so that
    // the parent gets the right data.
    if (!empty($this->options['expose']['identifier'])) {
      $value = &$input[$this->options['expose']['identifier']];
      if (!is_array($value)) {
        $value = array(
          'value' => $value,
        );
      }
    }
    // We artificially set values for min and max to make parents chacking function happy
    $value['min'] = 0;
    $value['max'] = 0;
    $rc = parent::accept_exposed_input($input);

    if (empty($this->options['expose']['required'])) {
      // We have to do some of our own checking for non-required filters.
      $info = $this->operators();
      if (!empty($info[$this->operator]['values'])) {
        switch ($info[$this->operator]['values']) {
          case 1:
            if ($value['value'] === '') {
              return FALSE;
            }
            break;
          case 2:
            if ($value['min'] === '' && $value['max'] === '') {
              return FALSE;
            }
            break;
        }
      }
    }

    return TRUE;
  }
}
