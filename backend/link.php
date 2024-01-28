<?php
// PHP code to receive and process the JSON data from JavaScript
chdir(__DIR__ . '/executables');

$input = file_get_contents('php://input'); // Get the raw POST data

//------!!IMPORTANT!!-----------//
$exePath = 'main_program.exe';

$arrayData = (array)json_decode($input, true); // Decode the JSON data into a PHP array

$stringinput = implode($arrayData); // array to string conversion

$command = "$exePath" . " $stringinput";

if ($arrayData !== null) {
    exec($command, $output, $return_val);
    if(!$return_val)
        echo implode("\n",$output);
    else
        http_response_code(400);
} else
    http_response_code(400);