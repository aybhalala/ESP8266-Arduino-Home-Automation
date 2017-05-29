<?php
$Current=$_POST{"current"};
$Human=$_POST{"human"};
$Write="<p>Current : " . $Current . "Amp</p>"."<p>Human Detected : " . $Human ."</p>";
file_put_contents('sensor.html',$Write);

if(strcmp($Human, 'Yes') == 0){
	$msg = "Human Activity Detected";

$msg = wordwrap($msg,70);

// send email
mail("yourmail@gmail.com","Human detected",$msg);
}


?>