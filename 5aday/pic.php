<?php
//echo time();
$first_time=1191011763;


$limit=(time()/60/60/24)-($first_time/60/60/24);
$limit=(int)$limit;
//The way flahs exports is beginning with 1, but keep it so that we can use 0 safely...
//$limit++;
//echo $limit;

if(!isset($_GET['day'])){
	$day=$limit;
}else{
	$day=$_GET['day'];
}

if($day>$limit || $day<0){
	exit();
}
$tmp=(int) $day;
$tmp++;

$fn="image/comic_000".$tmp.".png";

if($tmp>=10){
	$fn="image/comic_00".$tmp.".png";
}
if($tmp>=100){
	$fn="image/comic_0".$tmp.".png";
}
if($tmp>=1000){
	$fn="image/comic_".$tmp.".png";
}

$fh=fopen($fn,"rb");

if($fh==NULL){
	exit();
}
rewind($fh);
header("Content-Type: image/png");
header("Content-Length: " . filesize($fn));
fpassthru($fh);

fclose($fh);
exit();























?>
