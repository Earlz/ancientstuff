<?php
	include_once 'security.php';
	include_once 'config.php';



	$name_index=0;
	if(isset($_GET['cache'])==true && $_GET['cache']==1){
		//Find addon files
		$addlist=glob($addon_dir.'/*'.$addon_suffix);
		//foreach(glob('*_addon.php') as $fname){
		for($i=0;$i<count($addlist);$i++){
			//echo $addlist[$i];
			CacheAddon($addlist[$i]);
			$name_index++;

		}
		Cache();
	}
	//$current_page="test_addon.php"; //temp...
	if(isset($_GET['LID'])==true){
		include 'temp_argument.php';
		//print_r($test1);
	}
	LID_Init();
	include_once "cache/addon_tables.php";
	$niche_action='current';
	if(isset($_GET['page'])==true){
		$current_page=$_GET['page'];
		//include "$current_page"; //do this in theme...
	}else{
		$current_page="blank.php";
	}
	include_once $theme_file;
	//echo "<a href=\"".LID_Generate("test1",array("test1"=>array("test2"=>"test3")))."\">Test!</a>";
	LID_Exit();
















	exit();

	function CacheAddon($fn){
		global $niche_action,$niche_args,$add_file_table,$add_link_table,$add_title_table,$add_ver_table,$add_name_table,$name_index;
		$niche_action="cache";
		$niche_args=array(0=>0); //blank array
		include $fn;
		$add_file_table[$niche_name]=$fn; //create a list with addon-to-filename translation..
		$add_link_table[$niche_name]=$link_name;
		$add_title_table[$niche_name]=$page_title;
		$add_ver_table[$niche_name]=$version;
		$add_name_table[$name_index]=$niche_name;
		//echo $niche_name;

		return;
	}

	function Cache(){
	global $add_file_table,$add_link_table,$add_title_table,$add_ver_table,$add_name_table,$name_index;
		//chmod("cache/add_file_table.php",0777);
		$fh=fopen_per("cache/addon_tables.php","wb");
		fputs($fh,"<?php include_once 'security.php';\n");
		fputs($fh,"//This is an internal cache file for Niche, it should not be modified, and can not be executed directly\n");
		reset($add_file_table);
		for($i=0;$i<count($add_file_table);$i++){
			fputs($fh,'$add_file_table'."['".$add_name_table[$i]."']='".current($add_file_table)."';\n");
			next($add_file_table);
		}
		fputs($fh,'//$add_link_table[] cache'."\n");
		reset($add_link_table);
		for($i=0;$i<count($add_link_table);$i++){
			fputs($fh,'$add_link_table'."['".$add_name_table[$i]."']='".current($add_link_table)."';\n");
			next($add_link_table);
		}
		reset($add_title_table);
		fputs($fh,'//$add_title_table[] cache'."\n");
		for($i=0;$i<count($add_title_table);$i++){
			fputs($fh,'$add_title_table'."['".$add_name_table[$i]."']='".current($add_title_table)."';\n");
			next($add_title_table);
		}
		reset($add_ver_table);
		fputs($fh,'//$add_ver_table[] cache'."\n");
		for($i=0;$i<count($add_ver_table);$i++){
			fputs($fh,'$add_ver_table'."['".$add_name_table[$i]."']=".current($add_ver_table).";\n");
			next($add_file_table);
		}
		fputs($fh,'//$add_name_table[] cahce'."\n");
		for($i=0;$i<count($add_name_table);$i++){
			fputs($fh,'$add_name_table['.$i.']=\''.$add_name_table[$i]."';\n");
		}
		fputs($fh,"//END Cache File\n?>");



		fclose($fh);
	}
	function LID_Init(){
		global $arg_file_handle,$link_index;
		$arg_file_handle=fopen_per("temp_argument.php","wb");
		$tmp_write="<"."?php
						include_once 'security.php';
						".'if(isset($_GET['."'LID'])==TRUE){
						".'switch($_GET'."['LID']){\n";
		//have fun reading that up above....
		$link_index=0;
		fwrite($arg_file_handle,$tmp_write);
	}
	function LID_Exit(){
		global $arg_file_handle;
		fwrite($arg_file_handle,"default:\n");
		fwrite($arg_file_handle,"echo \"WARNING! LID not found: LID=\"".'.$_GET['."'LID'].\"<br><br>\";\n");
		fwrite($arg_file_handle,"break;\n");
		fwrite($arg_file_handle,"}}\n");
		fwrite($arg_file_handle,"?".">");
		fclose($arg_file_handle);
	}

	function LID_Generate($varname,$var,$page="blank.php"){
		global $current_page,$link_index,$arg_file_handle;
		if($page=="blank.php"){$page=$current_page;} //stupid PHP doesn't allow default args to be variable..
		$var=serialize($var);
		fwrite($arg_file_handle,"case $link_index:\n");
		fwrite($arg_file_handle,'$'."$varname=unserialize('$var');\n");
		fwrite($arg_file_handle,"break;\n\n");
		$tmp=$link_index;
		$link_index++;
		return "?page=".$page."&LID=".$tmp;
	}





	//****Idea Comment Block*****//
	//use Serialize for multiple types!!

	//Ok, Caching should not be done at startup, but should only be cached when requested..
	//With this, it enables us to only have to include an addon file once, and with the
	//request to "view" it.
	//There will be an install and uninstall command. Eventually, to install, what you will have to do
	//is
	//1. Upload addon file. (through FTP, or through an addon utility thing)
	//2. Goto the configuration addon(a special addon that is unremovable) and goto the install link
	//   or whatever. From there, you just type in the name of the file(or browse, or even upload the file form that)
	//3. Then, it will tell you to follow a link to recache, and then you will follow another link to the home
	//   with the navigation and such...
	//
	//From the pogrammer point of view...
	//1. Your addon file is included, and $niche_action=='install'
	//2. From here, you should copy your file into the $addon_dir folder..
	//3. Then, you should make any kinda cache or config files, and you can display things with simple html
	//   for display....NOTE: For any configuration that requires user input, you should do a first-run type thing...
	//If you have a need for multiple files(like an installer, and then a zip file, or the like..) then the
	//install addon file should download appropriate file(s) or have a prompt for the user to upload them.
	//Note: For user interaction, a first-run type thing should be used.

	//For navigation between addons and inside of addons..
	//I will use simple GET arguments. There will be two arguments passed by GET. The first
	//being the addon filename, and the second being the Link ID(LID), The LID is used as an index
	//with which to tell which argument to activate in the temp_argument.php file...
	//There will be two methods to generate LID arguments...
	//The first one is the more powerful one. It allows you to directly inject code along with variables
	//and arrays.
	//An example LID Argument using the open method...
	//lid_handle=LID_New();
	//echo "<a href=\"?page=$ThisPage&LID=$lid_handle\">My LID link</a>"; //the actual link
	//LID_InsertCode("//My Code\n");
	//LID_InsertVar("name_of_array",array("key1"=>"value"));
	//LID_InsertVar("name_of_variable",12345); //this must be a number..
	//LID_InsertVar("name_of_string","Yay! a string!");
	//LID_Close(); //no handle is involved, because you can only open one LID at a time..
	////:~
	//The other method is quite simpler, but gives you less power..(no code injection
	//echo "<a href=\"".LID_Generate(array("yay"=>"..."))."\">My Link</a>";
	//The LID_Generate function will return the GET values, and will generate the code in temp_argument.php
	//Note, all of these will probably be some pretty expensive functions...so, use only where needed..

?>


