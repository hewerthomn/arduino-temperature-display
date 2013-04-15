<?php
function get_temperature() {
    $json_string = file_get_contents("http://api.wunderground.com/api/b790f2291d7428ee/conditions/q/SBPV.json");
    $parsed_json = json_decode($json_string);
    $location  = $parsed_json->{'current_observation'}->{'display_location'}->{'city'};
    $location .= '/' . $parsed_json->{'current_observation'}->{'display_location'}->{'country_iso3166'};
    $temp_c = $parsed_json->{'current_observation'}->{'temp_c'};
    $relative_humidity = $parsed_json->{'current_observation'}->{'relative_humidity'};

    $location = str_pad($location, 16);
    $temperature  = str_pad("{$relative_humidity}H  {$temp_c}C", 11);

    return array(        
        'city' => $location,
        'temp' => $temperature,
        'time' => date('h:i')
    );
}