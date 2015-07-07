<?php
/**
 * This plugin array is more or less self documenting.
 */
$plugin = array(
  // the title in the admin
  'title' => t('Custom papne content type'),
  // no one knows if "single" defaults to FALSE...
  'single' => TRUE,
  // oh joy, I get my own section of panel panes
  'category' => array(t('Custom'), -9),
  'edit form' => 'paneCT_pane_content_type_edit_form',
  'render callback' => 'paneCT_pane_content_type_render'
);


/**
 * Run-time rendering of the body of the block (content type)
 * See ctools_plugin_examples for more advanced info.
 */
function paneCT_pane_content_type_render($subtype, $conf) {
  // our output is generate by js. Any markup or theme functions
  // could go here. 
  $block = '';
  $block->content = $conf['username'];
  return $block;
}


/**
 * 'Edit form' callback for the content type.
 */
function paneCT_pane_content_type_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];
  $form['username'] = array(
    '#type' => 'textfield',
    '#title' => t('username'),
    '#size' => 50,
    '#description' => t('A valid username.'),
    '#default_value' => !empty($conf['username']) ? $conf['username'] : 'Mega_user',
  );

  return $form;
}


/**
 * Submit function, note anything in the formstate[conf] automatically gets saved.
 */
function paneCT_pane_content_type_edit_form_submit(&$form, &$form_state) {
  $form_state['conf']['username'] = $form_state['values']['username'];
}

?>