<?php
	include_once "security.php";

	switch($niche_action){
		case "cache":
		$niche_name="Test Addon";
		$link_name="Test Addon Link";
		$page_title="Test Addon>>";
		$version=0.1;
		//echo 'Hai im cheezeburger??';
		break;

		case "current":
		echo 'HI!!!!<br>'."\n";
		echo "<a href=\"".LID_Generate("test1","my var")."\">My Test Link</a><br>\n";
		if(isset($test1)==true){echo "Test1=$test1<br>\n";}

		break;

		default:
		echo 'undefined action';
		break;
	}



?>
