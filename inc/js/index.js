page.preLoadEvent = function(e){
	if(e.index == 0){
		page.nav.init(document.getElementById("bar").getElementsByTagName("ul")[0]);
	}
};
page.target = "inc/page.php";
page.redirects = {
	"": "Home",
	"maps": "Maps/Overview",
	"mods": "Mods/Overview"
}