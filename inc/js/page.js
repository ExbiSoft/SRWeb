/*
** Written by Zaaryus
** Ajax page manager
*/

// Page object
var page = {
	_e: {
		loadIndex: 0
	},
	events: {
		_e: [],
		add: function(el, ev, fn){
			el.addEventListener(ev, fn, false);
			page.events._e.push({el: el, ev: ev, fn: fn});
		},
		remove: function(){
			for(var i = 0; i < page.events._e.length; i++){
				page.events._e[i].el.removeEventListener(page.events._e[i].ev, page.events._e[i].fn);
			}
			page.events._e = [];
		}
	},
	nav: {
		init: function(el){
			page.nav._e = el;
			page.nav._e.addEventListener("click", function(e){
				if(e.target.parentNode == el){
					var loc = e.target.className.split(" ")[0];
					loc = loc.replace(loc.split("_")[0]+"_", "").replace("_", "/");
					page.open(loc);
				}
			}, false);
		},
		change: function(){
			var child = page.nav._e.children, i;
			console.log("\n%cNavigation Event", "font-weight:bold;color:blue;");
			for(i = 0; i < child.length; i++){
				var loc = child[i].className.split(" ")[0];
				loc = loc.replace(loc.split("_")[0]+"_", "").replace("_", "/");
				if(page.location.indexOf(loc) == 0){
					console.log("\t%cPage location '"+page.location+"' starts with loc '"+loc+"'", "color:gray;");
					if(child[i].className.indexOf("active") == -1){
						console.log("\t\t%cAdds 'active' class to '"+child[i].className+"'", "color:gray;");
						child[i].className += " active";
					}
				}else if(child[i].className.indexOf("active") > -1){
					child[i].className = child[i].className.replace("active", "").trim();
					console.log("\t%cRemoves 'active' class from '"+child[i].className+"'", "color:gray;");
				}
			}
		}
	},
	update: function(){
		page.redirect(function(){
			if(page.preLoadEvent){
				page.preLoadEvent({page: page.location, index: page._e.loadIndex});
			}
			page.nav.change();
			page.request(function(res){
				page.events.remove();
				page.parse(res);
				if(page.pastLoadEvent){
					page.pastLoadEvent({page: page.location, index: page._e.loadIndex});
				}
				page._e.loadIndex++;
			});
		});
	},
	redirect: function(fn){
		page.preLocation = location.hash.replace(new RegExp("^[#/!]+|[#/!]+$", "g"), "");
		var loc = page.preLocation.toLowerCase();
		if(page.redirects && page.redirects[loc]){
			console.log("\n%cPage Redirect Event", "font-weight:bold;color:blue;");
			console.log("\t%cRedirected from '"+page.preLocation+"' to '"+page.redirects[loc]+"'", "color:gray;");
			loc = page.redirects[loc];
			page.open(loc);
		}else{
			page.location = location.hash.replace(new RegExp("^[#/!]+|[#/!]+$", "g"), "");
			if(page.preLocation == page.location){
				fn();
			}
		}
	},
	open: function(loc){
		location.hash = "!/"+loc;
	},
	request: function(fn){
		var xhr = new XMLHttpRequest();
		console.log("\n%cPage Request Event", "font-weight:bold;color:blue;");
		xhr.addEventListener("readystatechange", function(){
			if(xhr.readyState == 4){
				console.log("\t%cPage Requested succeeded", "color:green;");
				fn(JSON.parse(xhr.responseText));
			}
		}, false);
		try{
			xhr.open("POST", page.target);
			xhr.setRequestHeader("Content-Type","application/x-www-form-urlencoded");
			xhr.send("load="+page.location);
		}catch(e){
			console.log("\t%cPage Request failed", "color:red;");
		}
	},
	parse: function(data){
		console.log("\n%cPage Parse Event", "font-weight:bold;color:blue;");
		for(var i = 0; i < data.length; i++){
			page.parseNode(document.documentElement, data[i]);
		}
	},
	parseNode: function(parent, node){
		if(node.nodeName){
			var elem, key = Object.keys(node), i;
			switch(node.nodeName.toLowerCase()){
				case "head":
					elem = document.head;
					console.log("\t%cChanged node: head", "color:gray;");
					break;
				case "body":
					elem = document.body;
					console.log("\t%cChanged node: body", "color:gray;");
					break
				default:
					if(node.id && document.getElementById(node.id)){
						elem = document.getElementById(node.id);
						console.log("\t%cChanged node: "+node.nodeName+" (#"+node.id+")", "color:gray;");
					}else{
						elem = document.createElement(node.nodeName);
						parent.appendChild(elem);
						console.log("\t%cAdded node: "+node.nodeName+(node.id)?" (#"+node.id+")":"", "color:gray;");
					}
					break;
			}
			for(i = 0; i < key.length; i++){
				switch(key[i]){
					case "nodes":
						for(var it = 0; it < node[key[i]].length; it++){
							page.parseNode(elem, node[key[i]][it]);
						}
						break;
					case "clear":
						elem.innerHTML = "";
						console.log("\t%cCleared node: "+node.nodeName+(node.id)?" (#"+node.id+")":"", "color:gold;");
						break;
					case "style":
						var keys = Object.keys(node[key[i]]), it;
						for(it = 0; it < keys.length; it++){
							elem[key[i]][keys[it]] = node[key[i]][keys[it]];
						}
						break;
					default:
						elem[key[i]] = node[key[i]];
						break;
				}
			}
		}
	}
};

// Listeners
window.addEventListener("load", page.update, false);
window.addEventListener("hashchange", page.update, false);

// Alias for page.events.add
Object.prototype.addEvent = function(ev, fn){
    page.events.add(this, ev, fn);
}