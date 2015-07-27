<?php
	include_once "security.php";

	switch($niche_action){
		case "cache":
		$niche_name="Test Addon2";
		$link_name="Test Addon Link2";
		$page_title="Test Addon>>2";
		$version=0.2;
		//echo 'Hai im cheezeburger??';
		break;

		case "current":
		echo 'HI!!!!2<br>'."\n";
		echo "<a href=\"".LID_Generate("test2","my var")."\">My Test Link</a><br>\n";
		if(isset($test2)==true){echo "Test2=$test2<br>\n";}

		break;
		case "install":
		echo "Test2 Addon Install Successful<br>\n";
		break;
		case "uninstall":
		echo "Test2 Addon Uninstall Successful<br>\n";
		break;


		default:
		echo 'undefined action';
		break;
	}



?>
