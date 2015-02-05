<?php

function getPage($file){
	$file = "html/".str_replace("/", ".", strtolower($file)).".json";
	if(file_exists($file)){
		return file_get_contents($file);
	}else{
		return file_get_contents("html/error.json");
	}
}

if(isset($_POST['load'])){
	echo getPage($_POST['load']);
}

?>