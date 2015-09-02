<?php

/**
 * @file
 * Contains the basic 'node' field handler.
 */

/**
 * Field handler to provide simple renderer that allows linking to a ticket.
 * Definition terms:
 *
 * @ingroup views_field_handlers
 */
class views_handler_field_ticket extends views_handler_field {
  function render($values) {
    return $this->get_value($values);
  }
}
