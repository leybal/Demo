<?php
function jean_radios($variables) {
    $element = $variables ['element'];
    $attributes = array();
    if (isset($element ['#id'])) {
        $attributes ['id'] = $element ['#id'];
    }
    $attributes ['class'] = 'form-radios';
    if (!empty($element ['#attributes']['class'])) {
        $attributes ['class'] .= ' ' . implode(' ', $element ['#attributes']['class']);
    }
    else {
        $attributes ['class'] .= ' radios__custom';
    };
    if (isset($element ['#attributes']['title'])) {
        $attributes ['title'] = $element ['#attributes']['title'];
    }
    return '<div' . drupal_attributes($attributes) . '>' . (!empty($element ['#children']) ? $element ['#children'] : '') . '</div>';
}


?>