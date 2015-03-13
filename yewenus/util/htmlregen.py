#!/usr/bin/python
# -*- coding: utf-8 -*-

import sys

html_pre = '''<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<link rel="shortcut icon" href="/favicon.ico" />
<link href="styles/yewenus.css" rel="stylesheet" type="text/css" />
<title>%s - 笨狗随手留下</title>
<meta name="Description" content="%s" />
<script type="text/javascript" src="scripts/yewenus.js" charset="utf-8"></script>
</head>

<body>
<center>

<a name="page_top"></a>
<div id="bar">
  <center>
  <div id="bar_tab"></div>
  <div id="bar_body"></div>
  </center>
</div>

<div id="bart">%s</div>

<div id="main" align="left">

<div id="toc">
  <div id="tocController">
    <a href="javascript:void(0);" onclick="nav_hide();return false;">隐藏目录</a>
  </div>
  <div id="nav_ul">
'''

html_browser = '''
<div id="check_browser" ></div>
'''

html_tail = '''
</div>

<div id="ft">
  <hr width="1240px" size="0" />
  Powered by <a href="http://daringfireball.net/projects/markdown/">Markdown</a> and <a href="/about.html">yewen.us</a>.
  2011, <a href="http://hi.baidu.com/whusnoopy">Wen YE</a><br /><br />
</div>
</center>

<script type="text/javascript">
var _bdhmProtocol = (("https:" == document.location.protocol) ? " https://" : " http://");
document.write(unescape("%3Cscript src='" + _bdhmProtocol + "hm.baidu.com/h.js%3F6b3ab3c84fa75abb51bb8e42e6087b9d' type='text/javascript'%3E%3C/script%3E"));
</script>

</body>
</html>
'''


def main():

  out_str = ""

  incode = False

  nav_lev = 2
  nav = '''
    <ul>
      <li><a href="#page_top">回顶部</a></li>
'''
  buf = '<div id="content">\n'
  while True:
    line = sys.stdin.readline()
    if not line:
      break

    # 标题栏支持
    if line[:4] == "<h1>":
      title = line[4:-6]
      out_str += html_pre % (title, title, title)
      continue

    # 目录支持
    if line[0] == '<' and line[1] == 'h' and line[3] == '>':
      tag = line[4:-6]
      tag_href = tag.replace(' ', '_')
      buf += '\n<a name="%s"></a><a name="%s"></a>\n' % (tag_href, tag)

      if int(line[2]) < 4:
        level = int(line[2])

        if level > nav_lev:
          nav += "%s<ul>\n" % (' ' * 2 * level)
        if level < nav_lev:
          nav += "%s</ul>\n" % (' ' * 2 * nav_lev)
        nav_lev = level
        nav += ' %s<li><a href="#%s">%s</a></li>\n' % \
               (' ' * 2 * level, tag_href, tag)

    # 表格支持
    # TODO

    # syntax highlighter 支持
    if not incode and len(line) > 12 and line[0:12] == "<pre><code>%":
      incode = True
      lang = line[13:-1]
      line = '<pre class="brush: %s;">\n' % lang

    if incode and len(line) > 13 and line[0:13] == "</code></pre>":
      incode = False
      line = "</pre>\n"

    # Notice 支持
    if len(line) > 8 and line[0:8] == "<p><ntc>":
      tmp = line
      line = '<center><p><div class="ntc" style="width:80%%">' \
             + tmp[8:-11] + '</div></p></center>\n'

    # 红 code 部分支持
    while line.find('<rc>') != -1:
      tmp = line
      st = tmp.find('<rc>')
      ed = tmp.find('</rc>')
      line = tmp[0:st] + '<code style="color:red">' \
             + tmp[st + 4:ed] + '</code>' + tmp[ed + 5:]

    buf += line
  pass

  while nav_lev > 1:
    nav += "%s</ul>\n" % (' ' * 2 * nav_lev)
    nav_lev -= 1

  nav += '  </div>\n</div>\n'
  buf += '</div>\n'

  out_str += nav
  out_str += html_browser
  out_str += buf
  out_str += html_tail
  print out_str

  return 0


if __name__ == "__main__":
  sys.exit(main())
