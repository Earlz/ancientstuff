<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN"
 "http://www.w3.org/TR/html4/loose.dtd">
<html>
<!--This is a list of all the so far published comics...-->

<head>
<title>A List of Every Comic</title>
<link rel=StyleSheet href="style.css" type="text/css">
</head>

<body>
<?php
include_once "list.php";
$first_time=1191011763;


$limit=(time()/60/60/24)-($first_time/60/60/24);
$limit=(int)$limit;

?>
<div class=header>
Five A Day -- A never ending Comic Project...<br>
(A tangent of <a href="http://earlz.biz.tm">Earlz.biz.tm</a>)<br>
</div>

<div class=nav>
<a href="./">Back To Current</a>
</div>
<br>
<!--Firefox Bug fix... using the nav class...-->
<div class=comic_list>
<?php
for($i=0;$i<=$limit;$i++){
	echo "<a href=\"./?day=".$i."\">";
	echo "#".($i+1)."-";
	if(isset($clist[$i]->name)){
		echo $clist[$i]->name;
	}else{
		echo "Untitled";
	}
	echo "</a><br>\n";
}

?>

</div>



</body>
</html>

