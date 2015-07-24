<?php
function interval_select_views_data_alter(&$data) {
  $data['field_data_field_pv']['prix'] = array(
    'title' => t('Prix'),
    'group' => 'Annonces',
    'help' => t('Le prix de vente'),
   'real field' => 'field_pv_value',
   'filter' => array(
      'handler' => 'interval_select_handler_filter_interval_prix',
    ),
    );
  $data['field_data_field_surf_hab']['surface'] = array(
    'title' => t('Surface hab.'),
    'group' => 'Annonces',
    'help' => t('La surface habitable du bien'),
   'real field' => 'field_surf_hab_value',
   'filter' => array(
      'handler' => 'interval_select_handler_filter_interval_surface',
    ),        
  );    
}
//function interval_select_views_query_alter(&$view, &$query	 ) {
//  vd($query);
//}
