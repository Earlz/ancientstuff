<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN"
 "http://www.w3.org/TR/html4/loose.dtd">
<html>



<!--Ok, so there was this duck right? and it was all like ooh...lookie here at me...
so I looked, and the duck shot me in the face... Yes... true story, so then I calmly
walked to the nearest hospital while wondering if the droopage of my face was normal...-->

<!--The 5 A Day Site code... Now with handmade Hex color Combos!!-->
<head>
<?php
include_once "list.php";
$first_time=1191011763;


$limit=(time()/60/60/24)-($first_time/60/60/24);
$limit=(int)$limit;

if(!isset($_GET['day'])){
	$day=$limit;
}else{
	$day=$_GET['day'];
}
$day++;
//$limit++;
?>

<?php
if(isset($_POST['comment'])==true){ //then do guest book write

	if($_POST['verify']!=6){
		echo "[[Spam Prevention]]";
		exit();
	}
	//hacking prevention..

	$_POST['comment']=stripslashes($_POST['comment']); //undo stupid magic quote crap...@!$%
	$_POST['name']=stripslashes($_POST['name']);
	$_POST['website']=stripslashes($_POST['website']);
	$_POST['email']=stripslashes($_POST['email']);

	if(strlen($_POST['comment'])>1024*4){
			echo "Maximum content length is 4kb";
			exit();
	}
	if(strlen($_POST['name'])>512){
			echo "Maximum field length is 512b";
			exit();
	}
	if(strlen($_POST['website'])>512){
			echo "Maximum field length is 512b";
			exit();
	}
	if(strlen($_POST['email'])>512){
			echo "Maximum field length is 512b";
			exit();
	}

	if(($day-1)<=$limit){
	//echo "hi";
		//echo get_magic_quotes_gpc();
		//echo "yay";
		$edb_h=fopen("edb.php","ab");
		if($edb_h==false){
			echo "[[INTERNAL SERVER ERROR -- Failed fopening ebd]]";
			exit();
		}
		$vname='$'."edb_comic"."[".$_GET['day']."][".time()."]";
		fwrite($edb_h,"<?php\n".$vname."=new GuestBookStore;\n"); //write (vname)=new GuestBookStore;
		$name=strip_tags(addcslashes($_POST['name'],'\'')); //this is to prevent hacking..
		fwrite($edb_h,$vname."->name='".$name." ';\n"); //write (vname)->name="(name)";
		$website=strip_tags(addcslashes($_POST['website'],'\''));
		fwrite($edb_h,$vname."->website='".$website." ';\n");
		$email=strip_tags(addcslashes($_POST['email'],'\''));
		fwrite($edb_h,$vname."->email='".$email." ';\n");
		$comment=strip_tags(addcslashes($_POST['comment'],'\''));
		fwrite($edb_h,$vname."->content='".$comment." ';\n");
		if(isset($_POST['rating']) && $_POST['rating']!="None"){
			$tmp=(int)$_POST['rating'];
			if($tmp>=0 && $tmp<=5){
				fwrite($edb_h,$vname."->rating=".$tmp.";\n");
			}
		}
		fwrite($edb_h,"//added by guestbook form\n?>");
		fclose($edb_h);
	}
}
?>


<title>Five-A-Day -- Comic #<?echo $day;?></title>

<link rel=StyleSheet href="style.css" type="text/css">
</head>

<body>

<div class=header>
Five A Day -- A never ending Comic Project...<br>
(A tangent of <a href="http://earlz.biz.tm">Earlz.biz.tm</a>)<br>
</div>
<div class=nav>
<a href="?day=0">|&lt;First</a>


<a href="?day=<?if($day>1){echo $day-2;}else{echo 0;}?>">&lt;&lt; Previous</a>

<a href="archive.php">|Archive|</a>

<a href="?day=<?if($day<=($limit-1)){echo $day;}else{echo $limit;}?>">Next &gt;&gt;</a>


<a href="?">Current &gt;|</a>
</div>


<div class=header>
<br>
<?php
include_once "edb.php";
if(!isset($clist[($day-1)]->name)){
	echo "Untitled";
}else{
	echo $clist[($day-1)]->name."\n";
}
echo "</div>";
if(isset($clist[($day-1)]->note)){
	echo "<div class=note>\n".$clist[($day-1)]->note."<br>\n\n";
}else{
	echo "<div class=note>\nUnnoted...<br>\n";
}

if(isset($edb_comic[$day-1])){
	$c=0;
	$total=0;
	reset($edb_comic[$day-1]);
	for($i=0;$i<=count($edb_comic[$day-1]);$i++){
		if(isset($edb_comic[$day-1][key($edb_comic[$day-1])]->rating)){ //ohh yea...gotta love this stuff....
			$total=$total+$edb_comic[$day-1][key($edb_comic[$day-1])]->rating;
			$c++;
		}
		next($edb_comic[$day-1]);
	}
	if($c==0){
		echo "Not Rated...yet";
	}else{
		$tmp=(int)$total/$c;
		echo "Rating: ".$tmp;
	}


}else{
	echo "Not Rated...yet";
}



echo "</div>";
?>

<div class=comic>





<img src="pic.php?day=<?echo $day-1?>" alt="<?if(isset($clist[($day-1)]->alt)){echo $clist[($day-1)]->alt;}else{echo "Unalted";}?>">

</div>
<br>--Comments--<br><br>
<form name="guestform" action="?day=<?echo $day-1?>" method="POST">
The sum of two and four:
<input type="text" name="verify" value="" size="2"> (This is for spam prevention)<br>
Rating: <SELECT NAME=rating>
<OPTION>None</OPTION>
<OPTION>0</OPTION>
<OPTION>1</OPTION>
<OPTION>2</OPTION>
<OPTION>3</OPTION>
<OPTION>4</OPTION>
<OPTION>5</OPTION>
</SELECT><br>
Your name: <input type="text" name="name" value="" size="20"><br>
Your Email: <input type="text" name="email" value="" size="20"><br>
Your website URL: <input type="text" name="website" value="" size="20"><br>
Your Comment:<br>
<textarea rows="3" cols="30" name="comment"></textarea><br>
<input type="hidden" name="edb_name" value="<?php echo $edb_name?>">
<input type="submit" value="Submit"><br>
</form>
<br><br>

<?php

GuestView($edb_comic[$day-1]);
?>



<div class=copy>

Page design, all content within(unless otherwise noted), and general ideas by Jordan "Earlz" Earls<br>
&copy; Jordan "Earlz" Earls 2005-2007<br>

</div>



</body>
</html>
<?php
function GuestView(&$edb_name){

	if(isset($edb_name)==0){
		//echo "...";
		echo "No one has posted anything...yet, why not be the first?<br>";
		return;
	}
	end($edb_name); //we cycle through backwards so the first thing posted is hte last thing displayed
	for($i=0;$i<count($edb_name);$i++){
		$temp=current($edb_name);
		if($temp->content!=" "){
			echo "Name: ".$temp->name."<br>\n";
			echo "Email: ".$temp->email."<br>\n";
			echo "Website: ".$temp->website."<br>\n";
			echo $temp->content;
			echo "\n<p class=timestamp>[".date("F j, Y, g:i:s a",key($edb_name))."]</p><br>\n\n";
		}
		prev($edb_name);
	}
}

?>

