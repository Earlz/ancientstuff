<?php
	//CONFIGURATION FILE
	//this will eventually be automatically generated...
	$niche_user='test'; //The niche user name(currently, you can only have one user)
	$niche_pass=md5('test2'); //The md5 hash of the niche password
	$niche_login_time=3600; //time to stay logged in
	$addon_dir='addons'; //what the addon directory is named
	$addon_suffix='_addon.php'; //the ending for addon files
	$theme_file='test_theme.php'; //which theme file to use
	$niche_default_permissions=0777; //This is the default permission used in fopen_per

	$niche_warnings=true; //If niche specific warnings should be used (should be turned on only if developing, or having problems)
	$error_levels=E_ERROR|E_PARSE|E_CORE_ERROR|E_COMPILE_ERROR|E_USER_ERROR|E_RECOVERABLE_ERROR;


	//NOT user definable..
	$niche_core_version="v0.3.5"; //CONSTANT the niche core framework version
	//version number is in code-rewrite.feature-adds.security-fixes





?>
