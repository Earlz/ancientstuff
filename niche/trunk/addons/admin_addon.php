<?php
	include_once 'security.php';

	switch($niche_action){
		case "cache":
		$niche_name="Niche Admin";
		$link_name="Niche Administration";
		$page_title="Niche Administration";
		$version=0.1;
		break;

		case "current":
		if(isset($admin)==false){
			DisplayMain_admin();
		}else{
			switch($admin["action"]){
				case "install":
				if(isset($admin['file'])==true){
					//chmod($addon_dir,$niche_default_permission);
					if(copy($admin['file'],$addon_dir.'/'.basename($admin['file']))==false){
						NicheError("Niche Error: Cannot copy ".$admin['file']." to ".$addon_dir.'/'.basename($admin['file']));
					}else{
						$niche_action="install";
						include_once $addon_dir.'/'.basename($admin['file']);
						echo "\n\n<br><br><a href=\"?cache=1\">Complete install by recaching</a><br>\n";
					}
				}else{
					$files=glob("addon_installs/"."*".$addon_suffix);
					for($i=0;$i<count($files);$i++){
						echo "<a href=\"".LID_Generate("admin",array("action"=>"install","file"=>$files[$i]))."\">Install: ".$files[$i]."</a><br>\n";
					}
				}
				break;

				case "uninstall":
				if(isset($admin['file'])==true){
					$niche_action="uninstall";
					include_once $admin['file'];
					if($admin['file']==$addon_dir."/admin".$addon_suffix){NicheError("Not possible to uninstall the administration addon!");}
					chmod($admin['file'],0666);
					if(!unlink($admin['file'])){
						NicheError("Niche Error: Cannot delete ".$admin['file']);
					}
					echo "\n\n<br><br><a href=\"?cache=1\">Complete uninstall by recaching</a><br>\n";
				}else{
					for($i=0;$i<count($add_name_table);$i++){
						echo "<a href=\"".LID_Generate("admin",array("action"=>"uninstall","file"=>$add_file_table[$add_name_table[$i]]))."\">Uninstall: ".$add_name_table[$i]."</a><br>\n";
					}
				}




			}

		}

		break;

		case "uninstall":
		//Can't unisntall the administration addon!!
		NicheError("Not possible to uninstall the administration addon!");

	}




	function DisplayMain_admin(){
		echo "<a href=\"?cache=1\">Recache(you will leave this page)</a><br>\n";
		echo "<a href=\"".LID_Generate("admin",array("action"=>"install"))."\">Install an addon</a><br>\n";
		echo "<a href=\"".LID_Generate("admin",array("action"=>"uninstall"))."\">Uninstall an addon</a><br>\n";
		echo "<a href=\"".LID_Generate("admin","...")."\">...</a><br>\n";
	}


?>

<?php

//Idea Block...
//For install:
//First, the addon file is copied into the addon dir.
//second, Recache
//third, the file is included with niche_action="install"
//finally, a last recache..




?>

