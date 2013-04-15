<?php
/**
 * API Index
 */
header('Cache-Control: no-cache, must-revalidate');
header('Expires: Mon, 26 Jul 1997 05:00:00 GMT');
header('Content-type: application/json');

$service = $_GET['s'];
date_default_timezone_set('America/Porto_Velho');

require 'temperature.php';

$data = get_temperature();

$result = "<{$data['city']}|{$data['temp']}|{$data['time']}>";

die($result);