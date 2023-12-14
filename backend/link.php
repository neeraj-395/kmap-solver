<?php
// PHP code to receive and process the JSON data from JavaScript
chdir(__DIR__ . '/executables');

$input = file_get_contents('php://input'); // Get the raw POST data

//------!!IMPORTANT!!-----------//
$exePath = 'K-Map_Solver.exe'; // if you are using this software on Linux change it to "K-Map_Linux"

$arrayData = (array)json_decode($input, true); // Decode the JSON data into a PHP array

$stringinput = implode($arrayData); // array to string conversion

$command = realpath($exePath) . " $stringinput";

if ($arrayData !== null) {
    shell_exec($command);
    echo "Execution Done!!\n";
} else
    echo "Execution failed!!\n";