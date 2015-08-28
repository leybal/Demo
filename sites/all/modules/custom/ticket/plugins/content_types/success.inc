<?php

$plugin = array(
  'title' => t('Ticket success'),
  'single' => TRUE,
  'category' => t('Custom'),
  'edit form' => 'ticket_success_content_type_edit_form',
  'render callback' => 'ticket_success_content_type_render'
);

function ticket_success_content_type_render($subtype, $conf) {
  $block = '';

  $ticket_get_success = variable_get('ticket_get', 'NULL');
  if ( $ticket_get_success['role'] === 'admin') {
    $block->content = $conf['admin_success_text'];
  }
  elseif ($ticket_get_success['role'] === 'user') {
    $block->content = $conf['user_success_text'];
  }
  else {
    $block->content = 'Страница используеться для перенаправления пользоватилей после успешного заполнения формы с билетом.';
  }
  return $block;
}

function ticket_success_content_type_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];
  $form['admin_success_text'] = array(
    '#type' => 'textfield',
    '#title' => t('Text for admin'),
    '#size' => 50,
    '#default_value' => !empty($conf['admin_success_text']) ? $conf['admin_success_text'] : '',
  );
  $form['user_success_text'] = array(
    '#type' => 'textfield',
    '#title' => t('Text for user'),
    '#size' => 50,
    '#default_value' => !empty($conf['user_success_text']) ? $conf['user_success_text'] : '',
  );
  return $form;
}

function ticket_success_content_type_edit_form_submit(&$form, &$form_state) {
  $form_state['conf']['admin_success_text'] = $form_state['values']['admin_success_text'];
  $form_state['conf']['user_success_text']  = $form_state['values']['user_success_text'];

}
