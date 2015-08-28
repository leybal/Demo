<?php

$plugin = array(
  'title' => t('Ticket get form'),
  'single' => TRUE,
  'category' => array(t('Custom'), -9),
  'edit form' => 'ticket_content_type_edit_form',   // pane settings
  'render callback' => 'ticket_content_type_render' // content on page
);

function ticket_content_type_render($subtype, $conf, $args, $context) {
  $block = new stdClass();
  $block->title = t('Ticket form');
  $entity = entity_get_controller('ticket')->create();
  $form = drupal_get_form('ticket_form', $entity, $conf);
  $block->content = $form;
  return $block;
}

function ticket_content_type_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];
  $form['admin_redirect'] = array(
    '#type' => 'textfield',
    '#title' => t('Admin Redirect'),
    '#size' => 50,
    '#default_value' => !empty($conf['admin_redirect']) ? $conf['admin_redirect'] : '',
  );
  $form['user_redirect'] = array(
    '#type' => 'textfield',
    '#title' => t('User redirect'),
    '#size' => 50,
    '#default_value' => !empty($conf['user_redirect']) ? $conf['user_redirect'] : '',
  );
  return $form;
}

function ticket_content_type_edit_form_submit(&$form, &$form_state) {
  $form_state['conf']['admin_redirect'] = $form_state['values']['admin_redirect'];
  $form_state['conf']['user_redirect'] = $form_state['values']['user_redirect'];
}

/* CTools form alter */
function ticket_form_ticket_form_alter(&$form, &$form_state) {
  $form['#submit'][] = 'ticket_form_handler';
}

function ticket_form_handler(&$form, &$form_state) {
  // User rules
  global $user;
  if (in_array('administrator', $user->roles)) {
    $form_state['redirect'] = $form_state['build_info']['args'][1]['admin_redirect'];
    $ticket_get = array();
    $ticket_get['role'] = 'admin';
    variable_set('ticket_get', $ticket_get);
  } else {
    $form_state['redirect'] = $form_state['build_info']['args'][1]['user_redirect'];
    $ticket_get['role'] = 'user';
    variable_set('ticket_get', $ticket_get);
  }
}