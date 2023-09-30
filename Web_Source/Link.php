<?php
// PHP code to receive and process the JSON data from JavaScript
$input = file_get_contents('php://input'); // Get the raw POST data

$exePath = 'K-Map_Solver.exe'; // Replace with the actual path to your .exe file

chdir(__DIR__ . '/PHP_Executable');

$arrayData = json_decode($input, false); // Decode the JSON data into a PHP array

$stringinput = implode($arrayData); // array to string conversion

if ($arrayData !== null) {
    $pipe = popen($exePath, 'w');
    if ($pipe) {
        fwrite($pipe, $stringinput);
        pclose($pipe);
        echo "Execution Done!!";
    } else
        echo "Execution file opening failed!!";
} else
    echo "No valid JSON data received.";
