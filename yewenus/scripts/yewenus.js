
yewenus_tab_menu = [
  {
    header : ["首页", "index.html"],
    items : [
    ["快速链接", "快速链接"],
    ]
  },
  {
    header : ["Blog", "blog/"],
    items : []
  },
  {
    header : ["工作笔记", "work.html"],
    items : [
    ["Vim 相关", "Vim_相关"],
    ["Linux 笔记", "Linux_基本命令笔记"],
    ]
  },
  {
    header : ["闲暇娱乐", "funny.html"],
    items : [
    ["音乐", "音乐"],
    ["电影", "电影"],
    ["读书", "读书"],
    ]
  },
  {
    header : ["About", "about.html"],
    items : [
    ["关于本站", "关于本站"],
    ["每日进步", "每天进步一点点"],
    ]
  },
];

function set_bar() {
  var bar_tab = document.getElementById('bar_tab');
  var bar_menu = document.createElement("ul");
  bar_menu.id = "bar_header";
  bar_tab.appendChild(bar_menu);
  for (var idx in yewenus_tab_menu) {
    var tab_item = document.createElement("li");
    tab_text = yewenus_tab_menu[idx].header[0];
    tab_tar = yewenus_tab_menu[idx].header[1];
    tab_item.id = "bar_tab_" + tab_text;
    tab_item.innerHTML = '<a href="' + tab_tar + '" onMouseOver="yewenus_tab_hover(\'' + idx + '\')">' + tab_text + '</a>';
    bar_menu.appendChild(tab_item);
  }
}

yewenus_tab_hover = function(tab_idx) {

  var bar_body = document.getElementById('bar_body');

  if (document.getElementById('bar_menu')) {
    bar_body.removeChild(document.getElementById('bar_menu'));
  }

  var body_menu = document.createElement("ul");
  body_menu.id = "bar_menu";
  bar_body.appendChild(body_menu);

  var body_item = document.createElement("li");
  body_item.style.width = 64 + 96*tab_idx + 'px';
  body_item.innerHTML = '&nbsp;';
  body_menu.appendChild(body_item);

  for (var idx in yewenus_tab_menu[tab_idx].items) {
    var body_item = document.createElement("li");
    menu_text = yewenus_tab_menu[tab_idx].items[idx][0];
    menu_tar = yewenus_tab_menu[tab_idx].header[1] + "#" + yewenus_tab_menu[tab_idx].items[idx][1];
    body_item.innerHTML = '<a href="' + menu_tar + '">' + menu_text + '</a>';
    body_menu.appendChild(body_item);
  }

  bar_body.style.display = 'block';

}

yewenus_tab_hide = function() {
  var bar_body = document.getElementById('bar_body');
  var e = window.event || arguments[0];
  var fromTar = e.fromElement || e.target,
    toTar = e.toElement || e.relatedTarget;
  var _in = false,
    _out = false;
  while (fromTar) {
    if (fromTar.id == "bar_body") {
      _in = true;
      break;
    }
    fromTar = fromTar.parentNode;
  }
  while (toTar) {
    if (toTar.id == "bar_body") {
      _out = true;
      break;
    }
    toTar = toTar.parentNode;
  }
  if (_in && !_out) {
    bar_body.style.display = 'none';
  }
}

function check_browser() {
  element_cb = document.getElementById('check_browser');
  var msg = "";
  if (navigator.userAgent.indexOf("MSIE")>0) {
    if(navigator.userAgent.indexOf("MSIE 6.0")>0) { // IE6
      msg = '还在使用老掉牙的 IE6?'
    } 
    if(navigator.userAgent.indexOf("MSIE 7.0")>0) { // IE7
      msg = '还在使用老掉牙的 IE7?'
    } 
    if(navigator.userAgent.indexOf("MSIE 8.0")>0) { // IE8
      msg = '虽然 IE8 不算老, 但是对 HTML5+CSS3 的支持还是太差.'
    } 
  } else {
    element_cb.style.display = 'none';
  }
  element_cb.innerHTML = '<div class="ntc"><center>' + msg + ' 强烈推荐使用 <a href="http://www.google.com/chrome/eula.html?extra=devchannel&hl=zh-CN">Google Chrome Dev</a> 分支浏览本站</center></div>';
}

function set_nav() {
  all_width = document.body.scrollWidth;
  toc_right = Math.floor(all_width / 2 - 615) + 'px';
  document.getElementById('toc').style.right = toc_right;
}

function nav_show() {
  document.getElementById('nav_ul').style.display = 'block';
  document.getElementById('tocController').innerHTML = '<a href="javascript:void(0);" onclick="nav_hide();return false;">隐藏目录</a>';
  document.getElementById('toc').resizeHandle();
}

function nav_hide() {
  document.getElementById('nav_ul').style.display = 'none';
  document.getElementById('tocController').innerHTML = '<a href="javascript:void(0);" onclick="nav_show();return false;">显示目录</a>';
  document.getElementById('toc').resizeHandle();
}

window.onload = function() {
  check_browser();
  set_bar();
  set_nav();
//  document.body.onmouseout = yewenus_tab_hide;
}

