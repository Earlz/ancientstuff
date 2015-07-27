<?php
//Eventually need to add server-side session ID expiration...
	include_once 'config.php';
	$login=false;
	error_reporting($error_levels);
	//echo $_SERVER['REQUEST_URI'];

	//if(isset($_SERVER['REQUEST_TIME'])==false){$_SERVER['REQUEST_TIME']='null';} //prevents some notices..
	$cookie_name='niche_login'.$_SERVER['SERVER_NAME'].$_SERVER['SERVER_PORT'].dirname($_SERVER['SCRIPT_NAME']);
	if(isset($salt_id)==false){$salt_id=md5($niche_pass^$niche_user^PHP_VERSION);} //I like salt...
	if(isset($_POST['login'])){ //we are logging in...
	if($_POST['name']==$niche_user && md5($_POST['pass'])==$niche_pass){ //user and pass are correct..
		$tmp=setcookie($cookie_name,$salt_id,time()+$niche_login_time);
		SetSessionID($salt_id);
		//echo $tmp;
		$login=true;
	}else{
		echo "Incorrect Login<br><br>\n";
		LoginForm();
		$login=false;
		//exit();

	}




	}else{
		//print_r($_COOKIE);
		//echo '.'.$cookie_name;
		$cookie_name=str_replace(".","_",$cookie_name);
		if(isset($_COOKIE[$cookie_name])){
		//echo '.';

			if($_COOKIE[$cookie_name]==$salt_id){ //need to add expiration...
			//echo '.';
				$login=true;
			}
		}
	}

	if($login==false){
		LoginForm();
		exit();
	}
	//Cookie names:
	//'niche_login'.$_SERVER['SERVER_NAME'].$_SERVER['SERVER_PORT'].dirname($_SERVER['SCRIPT_NAME'])

	//with SCRIPT_NAME, it should always be the same as you can't directly execute an addon, so the addon
	//directory will never be the script path...

	//echo 'done?';
	function SetSessionID(){
		global $salt_id;
		$fhan=fopen_per("temp_sid.php","w");
		fputs($fhan,"<"."?php\n");
		//echo $salt_id;
		fputs($fhan,'$niche_sid=\''.$salt_id.'\';'."\n");
		fputs($fhan,"?".">");
		fclose($fhan);
	}




	function LoginForm(){
		echo "<form name=\"login\" action=\"".$_SERVER['REQUEST_URI']."\" method=\"POST\"><br>\n";
		echo "User Name: <input type=\"text\" name=\"name\" size=20><br>\n";
		echo "Password: <input type=\"password\" name=\"pass\" size=20><br>\n";
		echo "<input type=\"submit\" value=\"Login\">";
		echo "<input type=\"hidden\" name=\"login\" value=\"true\">\n";
		echo "</form><br>\n";
	}

	function fopen_per($file,$mode,$per=0){
		global $niche_default_permission;
		if($per==0){$per=$niche_default_permission;} //workaround for default variable variables..
		$handle=fopen($file,$mode);
		if($handle==false){
			$t=chmod($per);
			if($t==false){
				NicheWarning("Failed to change the permissions of $file to $per");
				return false;
				}
			$handle=fopen($file,$mode);
		}
		return $handle;
	}

	function NicheWarning($text){
		global $niche_warnings;
		if($niche_warnings==true){
			echo "Niche Warning! $text";
		}
	}

	function NicheError($text){
		//echo "Niche Error! $text";
		trigger_error($text,E_USER_ERROR);
	}







?>
