<?php
/**
 * API Index
 */
header('Cache-Control: no-cache, must-revalidate');
header('Expires: Mon, 26 Jul 1997 05:00:00 GMT');
header('Content-type: application/json');

$service = $_GET['s'];
date_default_timezone_set('America/Porto_Velho');

/**
 * Usa a API da wunderground para chckar a 
 * @return array
 */
function get_temperature() {
    /**
     * create a account and get your API key at
     * http://www.wunderground.com/
     */
    $APIKEY = "";
    //$json_string = file_get_contents("http://api.wunderground.com/api/{$APIKEY}/conditions/q/BR/Porto_Velho.json");
    $json_string = file_get_contents("http://api.wunderground.com/api/{$APIKEY}/conditions/q/SBPV.json");
    
    $parsed_json = json_decode($json_string);

    $location  = $parsed_json->{'current_observation'}->{'display_location'}->{'city'};
    $location .= '/' . $parsed_json->{'current_observation'}->{'display_location'}->{'country_iso3166'};
    $temp_c = $parsed_json->{'current_observation'}->{'temp_c'};
    $relative_humidity = $parsed_json->{'current_observation'}->{'relative_humidity'};

    /**
     * preenche com espaços se necessário até o tamanho indicado
     */
    $location = str_pad($location, 16);
    $temperature  = str_pad("{$relative_humidity} {$temp_c}C", 11);

    return array(        
        'city' => $location,
        'temp' => $temperature,
        'time' => date('h:i')
    );
}

$data = array(
 'city' => 'Porto Velho/RO  ',
 'temp' => '90%H  24C  ',
 'time' => date('h:i')
);

// $data = get_temperature();

$result = "<{$data['city']}|{$data['temp']}|{$data['time']}>";

die($result);