(function(global,factory){typeof exports==='object'&&typeof module!=='undefined'?module.exports=factory():typeof define==='function'&&define.amd?define(factory):(global.CodeMirror=factory());}(this,(function(){'use strict';var userAgent=navigator.userAgent
var platform=navigator.platform
var gecko=/gecko\/\d/i.test(userAgent)
var ie_upto10=/MSIE \d/.test(userAgent)
var ie_11up=/Trident\/(?:[7-9]|\d{2,})\..*rv:(\d+)/.exec(userAgent)
var edge=/Edge\/(\d+)/.exec(userAgent)
var ie=ie_upto10||ie_11up||edge
var ie_version=ie&&(ie_upto10?document.documentMode||6:+(edge||ie_11up)[1])
var webkit=!edge&&/WebKit\//.test(userAgent)
var qtwebkit=webkit&&/Qt\/\d+\.\d+/.test(userAgent)
var chrome=!edge&&/Chrome\//.test(userAgent)
var presto=/Opera\//.test(userAgent)
var safari=/Apple Computer/.test(navigator.vendor)
var mac_geMountainLion=/Mac OS X 1\d\D([8-9]|\d\d)\D/.test(userAgent)
var phantom=/PhantomJS/.test(userAgent)
var ios=!edge&&/AppleWebKit/.test(userAgent)&&/Mobile\/\w+/.test(userAgent)
var android=/Android/.test(userAgent)
var mobile=ios||android||/webOS|BlackBerry|Opera Mini|Opera Mobi|IEMobile/i.test(userAgent)
var mac=ios||/Mac/.test(platform)
var chromeOS=/\bCrOS\b/.test(userAgent)
var windows=/win/i.test(platform)
var presto_version=presto&&userAgent.match(/Version\/(\d*\.\d*)/)
if(presto_version){presto_version=Number(presto_version[1])}
if(presto_version&&presto_version>=15){presto=false;webkit=true}
var flipCtrlCmd=mac&&(qtwebkit||presto&&(presto_version==null||presto_version<12.11))
var captureRightClick=gecko||(ie&&ie_version>=9)
function classTest(cls){return new RegExp("(^|\\s)"+cls+"(?:$|\\s)\\s*")}
var rmClass=function(node,cls){var current=node.className
var match=classTest(cls).exec(current)
if(match){var after=current.slice(match.index+match[0].length)
node.className=current.slice(0,match.index)+(after?match[1]+after:"")}}
function removeChildren(e){for(var count=e.childNodes.length;count>0;--count)
{e.removeChild(e.firstChild)}
return e}
function removeChildrenAndAdd(parent,e){return removeChildren(parent).appendChild(e)}
function elt(tag,content,className,style){var e=document.createElement(tag)
if(className){e.className=className}
if(style){e.style.cssText=style}
if(typeof content=="string"){e.appendChild(document.createTextNode(content))}
else if(content){for(var i=0;i<content.length;++i){e.appendChild(content[i])}}
return e}
function eltP(tag,content,className,style){var e=elt(tag,content,className,style)
e.setAttribute("role","presentation")
return e}
var range
if(document.createRange){range=function(node,start,end,endNode){var r=document.createRange()
r.setEnd(endNode||node,end)
r.setStart(node,start)
return r}}
else{range=function(node,start,end){var r=document.body.createTextRange()
try{r.moveToElementText(node.parentNode)}
catch(e){return r}
r.collapse(true)
r.moveEnd("character",end)
r.moveStart("character",start)
return r}}
function contains(parent,child){if(child.nodeType==3)
{child=child.parentNode}
if(parent.contains)
{return parent.contains(child)}
do{if(child.nodeType==11){child=child.host}
if(child==parent){return true}}while(child=child.parentNode)}
function activeElt(){var activeElement
try{activeElement=document.activeElement}catch(e){activeElement=document.body||null}
while(activeElement&&activeElement.shadowRoot&&activeElement.shadowRoot.activeElement)
{activeElement=activeElement.shadowRoot.activeElement}
return activeElement}
function addClass(node,cls){var current=node.className
if(!classTest(cls).test(current)){node.className+=(current?" ":"")+cls}}
function joinClasses(a,b){var as=a.split(" ")
for(var i=0;i<as.length;i++)
{if(as[i]&&!classTest(as[i]).test(b)){b+=" "+as[i]}}
return b}
var selectInput=function(node){node.select()}
if(ios)
{selectInput=function(node){node.selectionStart=0;node.selectionEnd=node.value.length}}
else if(ie)
{selectInput=function(node){try{node.select()}catch(_e){}}}
function bind(f){var args=Array.prototype.slice.call(arguments,1)
return function(){return f.apply(null,args)}}
function copyObj(obj,target,overwrite){if(!target){target={}}
for(var prop in obj)
{if(obj.hasOwnProperty(prop)&&(overwrite!==false||!target.hasOwnProperty(prop)))
{target[prop]=obj[prop]}}
return target}
function countColumn(string,end,tabSize,startIndex,startValue){if(end==null){end=string.search(/[^\s\u00a0]/)
if(end==-1){end=string.length}}
for(var i=startIndex||0,n=startValue||0;;){var nextTab=string.indexOf("\t",i)
if(nextTab<0||nextTab>=end)
{return n+(end-i)}
n+=nextTab-i
n+=tabSize-(n%tabSize)
i=nextTab+1}}
var Delayed=function Delayed(){this.id=null};Delayed.prototype.set=function set(ms,f){clearTimeout(this.id)
this.id=setTimeout(f,ms)};function indexOf(array,elt){for(var i=0;i<array.length;++i)
{if(array[i]==elt){return i}}
return-1}
var scrollerGap=30
var Pass={toString:function(){return"CodeMirror.Pass"}}
var sel_dontScroll={scroll:false};var sel_mouse={origin:"*mouse"};var sel_move={origin:"+move"};function findColumn(string,goal,tabSize){for(var pos=0,col=0;;){var nextTab=string.indexOf("\t",pos)
if(nextTab==-1){nextTab=string.length}
var skipped=nextTab-pos
if(nextTab==string.length||col+skipped>=goal)
{return pos+Math.min(skipped,goal-col)}
col+=nextTab-pos
col+=tabSize-(col%tabSize)
pos=nextTab+1
if(col>=goal){return pos}}}
var spaceStrs=[""]
function spaceStr(n){while(spaceStrs.length<=n)
{spaceStrs.push(lst(spaceStrs)+" ")}
return spaceStrs[n]}
function lst(arr){return arr[arr.length-1]}
function map(array,f){var out=[]
for(var i=0;i<array.length;i++){out[i]=f(array[i],i)}
return out}
function insertSorted(array,value,score){var pos=0,priority=score(value)
while(pos<array.length&&score(array[pos])<=priority){pos++}
array.splice(pos,0,value)}
function nothing(){}
function createObj(base,props){var inst
if(Object.create){inst=Object.create(base)}else{nothing.prototype=base
inst=new nothing()}
if(props){copyObj(props,inst)}
return inst}
var nonASCIISingleCaseWordChar=/[\u00df\u0587\u0590-\u05f4\u0600-\u06ff\u3040-\u309f\u30a0-\u30ff\u3400-\u4db5\u4e00-\u9fcc\uac00-\ud7af]/
function isWordCharBasic(ch){return/\w/.test(ch)||ch>"\x80"&&(ch.toUpperCase()!=ch.toLowerCase()||nonASCIISingleCaseWordChar.test(ch))}
function isWordChar(ch,helper){if(!helper){return isWordCharBasic(ch)}
if(helper.source.indexOf("\\w")>-1&&isWordCharBasic(ch)){return true}
return helper.test(ch)}
function isEmpty(obj){for(var n in obj){if(obj.hasOwnProperty(n)&&obj[n]){return false}}
return true}
var extendingChars=/[\u0300-\u036f\u0483-\u0489\u0591-\u05bd\u05bf\u05c1\u05c2\u05c4\u05c5\u05c7\u0610-\u061a\u064b-\u065e\u0670\u06d6-\u06dc\u06de-\u06e4\u06e7\u06e8\u06ea-\u06ed\u0711\u0730-\u074a\u07a6-\u07b0\u07eb-\u07f3\u0816-\u0819\u081b-\u0823\u0825-\u0827\u0829-\u082d\u0900-\u0902\u093c\u0941-\u0948\u094d\u0951-\u0955\u0962\u0963\u0981\u09bc\u09be\u09c1-\u09c4\u09cd\u09d7\u09e2\u09e3\u0a01\u0a02\u0a3c\u0a41\u0a42\u0a47\u0a48\u0a4b-\u0a4d\u0a51\u0a70\u0a71\u0a75\u0a81\u0a82\u0abc\u0ac1-\u0ac5\u0ac7\u0ac8\u0acd\u0ae2\u0ae3\u0b01\u0b3c\u0b3e\u0b3f\u0b41-\u0b44\u0b4d\u0b56\u0b57\u0b62\u0b63\u0b82\u0bbe\u0bc0\u0bcd\u0bd7\u0c3e-\u0c40\u0c46-\u0c48\u0c4a-\u0c4d\u0c55\u0c56\u0c62\u0c63\u0cbc\u0cbf\u0cc2\u0cc6\u0ccc\u0ccd\u0cd5\u0cd6\u0ce2\u0ce3\u0d3e\u0d41-\u0d44\u0d4d\u0d57\u0d62\u0d63\u0dca\u0dcf\u0dd2-\u0dd4\u0dd6\u0ddf\u0e31\u0e34-\u0e3a\u0e47-\u0e4e\u0eb1\u0eb4-\u0eb9\u0ebb\u0ebc\u0ec8-\u0ecd\u0f18\u0f19\u0f35\u0f37\u0f39\u0f71-\u0f7e\u0f80-\u0f84\u0f86\u0f87\u0f90-\u0f97\u0f99-\u0fbc\u0fc6\u102d-\u1030\u1032-\u1037\u1039\u103a\u103d\u103e\u1058\u1059\u105e-\u1060\u1071-\u1074\u1082\u1085\u1086\u108d\u109d\u135f\u1712-\u1714\u1732-\u1734\u1752\u1753\u1772\u1773\u17b7-\u17bd\u17c6\u17c9-\u17d3\u17dd\u180b-\u180d\u18a9\u1920-\u1922\u1927\u1928\u1932\u1939-\u193b\u1a17\u1a18\u1a56\u1a58-\u1a5e\u1a60\u1a62\u1a65-\u1a6c\u1a73-\u1a7c\u1a7f\u1b00-\u1b03\u1b34\u1b36-\u1b3a\u1b3c\u1b42\u1b6b-\u1b73\u1b80\u1b81\u1ba2-\u1ba5\u1ba8\u1ba9\u1c2c-\u1c33\u1c36\u1c37\u1cd0-\u1cd2\u1cd4-\u1ce0\u1ce2-\u1ce8\u1ced\u1dc0-\u1de6\u1dfd-\u1dff\u200c\u200d\u20d0-\u20f0\u2cef-\u2cf1\u2de0-\u2dff\u302a-\u302f\u3099\u309a\ua66f-\ua672\ua67c\ua67d\ua6f0\ua6f1\ua802\ua806\ua80b\ua825\ua826\ua8c4\ua8e0-\ua8f1\ua926-\ua92d\ua947-\ua951\ua980-\ua982\ua9b3\ua9b6-\ua9b9\ua9bc\uaa29-\uaa2e\uaa31\uaa32\uaa35\uaa36\uaa43\uaa4c\uaab0\uaab2-\uaab4\uaab7\uaab8\uaabe\uaabf\uaac1\uabe5\uabe8\uabed\udc00-\udfff\ufb1e\ufe00-\ufe0f\ufe20-\ufe26\uff9e\uff9f]/
function isExtendingChar(ch){return ch.charCodeAt(0)>=768&&extendingChars.test(ch)}
function skipExtendingChars(str,pos,dir){while((dir<0?pos>0:pos<str.length)&&isExtendingChar(str.charAt(pos))){pos+=dir}
return pos}
function findFirst(pred,from,to){for(;;){if(Math.abs(from-to)<=1){return pred(from)?from:to}
var mid=Math.floor((from+to)/2)
if(pred(mid)){to=mid}
else{from=mid}}}
function Display(place,doc,input){var d=this
this.input=input
d.scrollbarFiller=elt("div",null,"CodeMirror-scrollbar-filler")
d.scrollbarFiller.setAttribute("cm-not-content","true")
d.gutterFiller=elt("div",null,"CodeMirror-gutter-filler")
d.gutterFiller.setAttribute("cm-not-content","true")
d.lineDiv=eltP("div",null,"CodeMirror-code")
d.selectionDiv=elt("div",null,null,"position: relative; z-index: 1")
d.cursorDiv=elt("div",null,"CodeMirror-cursors")
d.measure=elt("div",null,"CodeMirror-measure")
d.lineMeasure=elt("div",null,"CodeMirror-measure")
d.lineSpace=eltP("div",[d.measure,d.lineMeasure,d.selectionDiv,d.cursorDiv,d.lineDiv],null,"position: relative; outline: none")
var lines=eltP("div",[d.lineSpace],"CodeMirror-lines")
d.mover=elt("div",[lines],null,"position: relative")
d.sizer=elt("div",[d.mover],"CodeMirror-sizer")
d.sizerWidth=null
d.heightForcer=elt("div",null,null,"position: absolute; height: "+scrollerGap+"px; width: 1px;")
d.gutters=elt("div",null,"CodeMirror-gutters")
d.lineGutter=null
d.scroller=elt("div",[d.sizer,d.heightForcer,d.gutters],"CodeMirror-scroll")
d.scroller.setAttribute("tabIndex","-1")
d.wrapper=elt("div",[d.scrollbarFiller,d.gutterFiller,d.scroller],"CodeMirror")
if(ie&&ie_version<8){d.gutters.style.zIndex=-1;d.scroller.style.paddingRight=0}
if(!webkit&&!(gecko&&mobile)){d.scroller.draggable=true}
if(place){if(place.appendChild){place.appendChild(d.wrapper)}
else{place(d.wrapper)}}
d.viewFrom=d.viewTo=doc.first
d.reportedViewFrom=d.reportedViewTo=doc.first
d.view=[]
d.renderedView=null
d.externalMeasured=null
d.viewOffset=0
d.lastWrapHeight=d.lastWrapWidth=0
d.updateLineNumbers=null
d.nativeBarWidth=d.barHeight=d.barWidth=0
d.scrollbarsClipped=false
d.lineNumWidth=d.lineNumInnerWidth=d.lineNumChars=null
d.alignWidgets=false
d.cachedCharWidth=d.cachedTextHeight=d.cachedPaddingH=null
d.maxLine=null
d.maxLineLength=0
d.maxLineChanged=false
d.wheelDX=d.wheelDY=d.wheelStartX=d.wheelStartY=null
d.shift=false
d.selForContextMenu=null
d.activeTouch=null
input.init(d)}
function getLine(doc,n){n-=doc.first
if(n<0||n>=doc.size){throw new Error("There is no line "+(n+doc.first)+" in the document.")}
var chunk=doc
while(!chunk.lines){for(var i=0;;++i){var child=chunk.children[i],sz=child.chunkSize()
if(n<sz){chunk=child;break}
n-=sz}}
return chunk.lines[n]}
function getBetween(doc,start,end){var out=[],n=start.line
doc.iter(start.line,end.line+1,function(line){var text=line.text
if(n==end.line){text=text.slice(0,end.ch)}
if(n==start.line){text=text.slice(start.ch)}
out.push(text)
++n})
return out}
function getLines(doc,from,to){var out=[]
doc.iter(from,to,function(line){out.push(line.text)})
return out}
function updateLineHeight(line,height){var diff=height-line.height
if(diff){for(var n=line;n;n=n.parent){n.height+=diff}}}
function lineNo(line){if(line.parent==null){return null}
var cur=line.parent,no=indexOf(cur.lines,line)
for(var chunk=cur.parent;chunk;cur=chunk,chunk=chunk.parent){for(var i=0;;++i){if(chunk.children[i]==cur){break}
no+=chunk.children[i].chunkSize()}}
return no+cur.first}
function lineAtHeight(chunk,h){var n=chunk.first
outer:do{for(var i$1=0;i$1<chunk.children.length;++i$1){var child=chunk.children[i$1],ch=child.height
if(h<ch){chunk=child;continue outer}
h-=ch
n+=child.chunkSize()}
return n}while(!chunk.lines)
var i=0
for(;i<chunk.lines.length;++i){var line=chunk.lines[i],lh=line.height
if(h<lh){break}
h-=lh}
return n+i}
function isLine(doc,l){return l>=doc.first&&l<doc.first+doc.size}
function lineNumberFor(options,i){return String(options.lineNumberFormatter(i+options.firstLineNumber))}
function Pos(line,ch,sticky){if(sticky===void 0)sticky=null;if(!(this instanceof Pos)){return new Pos(line,ch,sticky)}
this.line=line
this.ch=ch
this.sticky=sticky}
function cmp(a,b){return a.line-b.line||a.ch-b.ch}
function equalCursorPos(a,b){return a.sticky==b.sticky&&cmp(a,b)==0}
function copyPos(x){return Pos(x.line,x.ch)}
function maxPos(a,b){return cmp(a,b)<0?b:a}
function minPos(a,b){return cmp(a,b)<0?a:b}
function clipLine(doc,n){return Math.max(doc.first,Math.min(n,doc.first+doc.size-1))}
function clipPos(doc,pos){if(pos.line<doc.first){return Pos(doc.first,0)}
var last=doc.first+doc.size-1
if(pos.line>last){return Pos(last,getLine(doc,last).text.length)}
return clipToLen(pos,getLine(doc,pos.line).text.length)}
function clipToLen(pos,linelen){var ch=pos.ch
if(ch==null||ch>linelen){return Pos(pos.line,linelen)}
else if(ch<0){return Pos(pos.line,0)}
else{return pos}}
function clipPosArray(doc,array){var out=[]
for(var i=0;i<array.length;i++){out[i]=clipPos(doc,array[i])}
return out}
var sawReadOnlySpans=false;var sawCollapsedSpans=false;function seeReadOnlySpans(){sawReadOnlySpans=true}
function seeCollapsedSpans(){sawCollapsedSpans=true}
function MarkedSpan(marker,from,to){this.marker=marker
this.from=from;this.to=to}
function getMarkedSpanFor(spans,marker){if(spans){for(var i=0;i<spans.length;++i){var span=spans[i]
if(span.marker==marker){return span}}}}
function removeMarkedSpan(spans,span){var r
for(var i=0;i<spans.length;++i)
{if(spans[i]!=span){(r||(r=[])).push(spans[i])}}
return r}
function addMarkedSpan(line,span){line.markedSpans=line.markedSpans?line.markedSpans.concat([span]):[span]
span.marker.attachLine(line)}
function markedSpansBefore(old,startCh,isInsert){var nw
if(old){for(var i=0;i<old.length;++i){var span=old[i],marker=span.marker
var startsBefore=span.from==null||(marker.inclusiveLeft?span.from<=startCh:span.from<startCh)
if(startsBefore||span.from==startCh&&marker.type=="bookmark"&&(!isInsert||!span.marker.insertLeft)){var endsAfter=span.to==null||(marker.inclusiveRight?span.to>=startCh:span.to>startCh);(nw||(nw=[])).push(new MarkedSpan(marker,span.from,endsAfter?null:span.to))}}}
return nw}
function markedSpansAfter(old,endCh,isInsert){var nw
if(old){for(var i=0;i<old.length;++i){var span=old[i],marker=span.marker
var endsAfter=span.to==null||(marker.inclusiveRight?span.to>=endCh:span.to>endCh)
if(endsAfter||span.from==endCh&&marker.type=="bookmark"&&(!isInsert||span.marker.insertLeft)){var startsBefore=span.from==null||(marker.inclusiveLeft?span.from<=endCh:span.from<endCh);(nw||(nw=[])).push(new MarkedSpan(marker,startsBefore?null:span.from-endCh,span.to==null?null:span.to-endCh))}}}
return nw}
function stretchSpansOverChange(doc,change){if(change.full){return null}
var oldFirst=isLine(doc,change.from.line)&&getLine(doc,change.from.line).markedSpans
var oldLast=isLine(doc,change.to.line)&&getLine(doc,change.to.line).markedSpans
if(!oldFirst&&!oldLast){return null}
var startCh=change.from.ch,endCh=change.to.ch,isInsert=cmp(change.from,change.to)==0
var first=markedSpansBefore(oldFirst,startCh,isInsert)
var last=markedSpansAfter(oldLast,endCh,isInsert)
var sameLine=change.text.length==1,offset=lst(change.text).length+(sameLine?startCh:0)
if(first){for(var i=0;i<first.length;++i){var span=first[i]
if(span.to==null){var found=getMarkedSpanFor(last,span.marker)
if(!found){span.to=startCh}
else if(sameLine){span.to=found.to==null?null:found.to+offset}}}}
if(last){for(var i$1=0;i$1<last.length;++i$1){var span$1=last[i$1]
if(span$1.to!=null){span$1.to+=offset}
if(span$1.from==null){var found$1=getMarkedSpanFor(first,span$1.marker)
if(!found$1){span$1.from=offset
if(sameLine){(first||(first=[])).push(span$1)}}}else{span$1.from+=offset
if(sameLine){(first||(first=[])).push(span$1)}}}}
if(first){first=clearEmptySpans(first)}
if(last&&last!=first){last=clearEmptySpans(last)}
var newMarkers=[first]
if(!sameLine){var gap=change.text.length-2,gapMarkers
if(gap>0&&first)
{for(var i$2=0;i$2<first.length;++i$2)
{if(first[i$2].to==null)
{(gapMarkers||(gapMarkers=[])).push(new MarkedSpan(first[i$2].marker,null,null))}}}
for(var i$3=0;i$3<gap;++i$3)
{newMarkers.push(gapMarkers)}
newMarkers.push(last)}
return newMarkers}
function clearEmptySpans(spans){for(var i=0;i<spans.length;++i){var span=spans[i]
if(span.from!=null&&span.from==span.to&&span.marker.clearWhenEmpty!==false)
{spans.splice(i--,1)}}
if(!spans.length){return null}
return spans}
function removeReadOnlyRanges(doc,from,to){var markers=null
doc.iter(from.line,to.line+1,function(line){if(line.markedSpans){for(var i=0;i<line.markedSpans.length;++i){var mark=line.markedSpans[i].marker
if(mark.readOnly&&(!markers||indexOf(markers,mark)==-1))
{(markers||(markers=[])).push(mark)}}}})
if(!markers){return null}
var parts=[{from:from,to:to}]
for(var i=0;i<markers.length;++i){var mk=markers[i],m=mk.find(0)
for(var j=0;j<parts.length;++j){var p=parts[j]
if(cmp(p.to,m.from)<0||cmp(p.from,m.to)>0){continue}
var newParts=[j,1],dfrom=cmp(p.from,m.from),dto=cmp(p.to,m.to)
if(dfrom<0||!mk.inclusiveLeft&&!dfrom)
{newParts.push({from:p.from,to:m.from})}
if(dto>0||!mk.inclusiveRight&&!dto)
{newParts.push({from:m.to,to:p.to})}
parts.splice.apply(parts,newParts)
j+=newParts.length-3}}
return parts}
function detachMarkedSpans(line){var spans=line.markedSpans
if(!spans){return}
for(var i=0;i<spans.length;++i)
{spans[i].marker.detachLine(line)}
line.markedSpans=null}
function attachMarkedSpans(line,spans){if(!spans){return}
for(var i=0;i<spans.length;++i)
{spans[i].marker.attachLine(line)}
line.markedSpans=spans}
function extraLeft(marker){return marker.inclusiveLeft?-1:0}
function extraRight(marker){return marker.inclusiveRight?1:0}
function compareCollapsedMarkers(a,b){var lenDiff=a.lines.length-b.lines.length
if(lenDiff!=0){return lenDiff}
var aPos=a.find(),bPos=b.find()
var fromCmp=cmp(aPos.from,bPos.from)||extraLeft(a)-extraLeft(b)
if(fromCmp){return-fromCmp}
var toCmp=cmp(aPos.to,bPos.to)||extraRight(a)-extraRight(b)
if(toCmp){return toCmp}
return b.id-a.id}
function collapsedSpanAtSide(line,start){var sps=sawCollapsedSpans&&line.markedSpans,found
if(sps){for(var sp=(void 0),i=0;i<sps.length;++i){sp=sps[i]
if(sp.marker.collapsed&&(start?sp.from:sp.to)==null&&(!found||compareCollapsedMarkers(found,sp.marker)<0))
{found=sp.marker}}}
return found}
function collapsedSpanAtStart(line){return collapsedSpanAtSide(line,true)}
function collapsedSpanAtEnd(line){return collapsedSpanAtSide(line,false)}
function conflictingCollapsedRange(doc,lineNo,from,to,marker){var line=getLine(doc,lineNo)
var sps=sawCollapsedSpans&&line.markedSpans
if(sps){for(var i=0;i<sps.length;++i){var sp=sps[i]
if(!sp.marker.collapsed){continue}
var found=sp.marker.find(0)
var fromCmp=cmp(found.from,from)||extraLeft(sp.marker)-extraLeft(marker)
var toCmp=cmp(found.to,to)||extraRight(sp.marker)-extraRight(marker)
if(fromCmp>=0&&toCmp<=0||fromCmp<=0&&toCmp>=0){continue}
if(fromCmp<=0&&(sp.marker.inclusiveRight&&marker.inclusiveLeft?cmp(found.to,from)>=0:cmp(found.to,from)>0)||fromCmp>=0&&(sp.marker.inclusiveRight&&marker.inclusiveLeft?cmp(found.from,to)<=0:cmp(found.from,to)<0))
{return true}}}}
function visualLine(line){var merged
while(merged=collapsedSpanAtStart(line))
{line=merged.find(-1,true).line}
return line}
function visualLineEnd(line){var merged
while(merged=collapsedSpanAtEnd(line))
{line=merged.find(1,true).line}
return line}
function visualLineContinued(line){var merged,lines
while(merged=collapsedSpanAtEnd(line)){line=merged.find(1,true).line;(lines||(lines=[])).push(line)}
return lines}
function visualLineNo(doc,lineN){var line=getLine(doc,lineN),vis=visualLine(line)
if(line==vis){return lineN}
return lineNo(vis)}
function visualLineEndNo(doc,lineN){if(lineN>doc.lastLine()){return lineN}
var line=getLine(doc,lineN),merged
if(!lineIsHidden(doc,line)){return lineN}
while(merged=collapsedSpanAtEnd(line))
{line=merged.find(1,true).line}
return lineNo(line)+1}
function lineIsHidden(doc,line){var sps=sawCollapsedSpans&&line.markedSpans
if(sps){for(var sp=(void 0),i=0;i<sps.length;++i){sp=sps[i]
if(!sp.marker.collapsed){continue}
if(sp.from==null){return true}
if(sp.marker.widgetNode){continue}
if(sp.from==0&&sp.marker.inclusiveLeft&&lineIsHiddenInner(doc,line,sp))
{return true}}}}
function lineIsHiddenInner(doc,line,span){if(span.to==null){var end=span.marker.find(1,true)
return lineIsHiddenInner(doc,end.line,getMarkedSpanFor(end.line.markedSpans,span.marker))}
if(span.marker.inclusiveRight&&span.to==line.text.length)
{return true}
for(var sp=(void 0),i=0;i<line.markedSpans.length;++i){sp=line.markedSpans[i]
if(sp.marker.collapsed&&!sp.marker.widgetNode&&sp.from==span.to&&(sp.to==null||sp.to!=span.from)&&(sp.marker.inclusiveLeft||span.marker.inclusiveRight)&&lineIsHiddenInner(doc,line,sp)){return true}}}
function heightAtLine(lineObj){lineObj=visualLine(lineObj)
var h=0,chunk=lineObj.parent
for(var i=0;i<chunk.lines.length;++i){var line=chunk.lines[i]
if(line==lineObj){break}
else{h+=line.height}}
for(var p=chunk.parent;p;chunk=p,p=chunk.parent){for(var i$1=0;i$1<p.children.length;++i$1){var cur=p.children[i$1]
if(cur==chunk){break}
else{h+=cur.height}}}
return h}
function lineLength(line){if(line.height==0){return 0}
var len=line.text.length,merged,cur=line
while(merged=collapsedSpanAtStart(cur)){var found=merged.find(0,true)
cur=found.from.line
len+=found.from.ch-found.to.ch}
cur=line
while(merged=collapsedSpanAtEnd(cur)){var found$1=merged.find(0,true)
len-=cur.text.length-found$1.from.ch
cur=found$1.to.line
len+=cur.text.length-found$1.to.ch}
return len}
function findMaxLine(cm){var d=cm.display,doc=cm.doc
d.maxLine=getLine(doc,doc.first)
d.maxLineLength=lineLength(d.maxLine)
d.maxLineChanged=true
doc.iter(function(line){var len=lineLength(line)
if(len>d.maxLineLength){d.maxLineLength=len
d.maxLine=line}})}
function iterateBidiSections(order,from,to,f){if(!order){return f(from,to,"ltr")}
var found=false
for(var i=0;i<order.length;++i){var part=order[i]
if(part.from<to&&part.to>from||from==to&&part.to==from){f(Math.max(part.from,from),Math.min(part.to,to),part.level==1?"rtl":"ltr")
found=true}}
if(!found){f(from,to,"ltr")}}
var bidiOther=null
function getBidiPartAt(order,ch,sticky){var found
bidiOther=null
for(var i=0;i<order.length;++i){var cur=order[i]
if(cur.from<ch&&cur.to>ch){return i}
if(cur.to==ch){if(cur.from!=cur.to&&sticky=="before"){found=i}
else{bidiOther=i}}
if(cur.from==ch){if(cur.from!=cur.to&&sticky!="before"){found=i}
else{bidiOther=i}}}
return found!=null?found:bidiOther}
var bidiOrdering=(function(){var lowTypes="bbbbbbbbbtstwsbbbbbbbbbbbbbbssstwNN%%%NNNNNN,N,N1111111111NNNNNNNLLLLLLLLLLLLLLLLLLLLLLLLLLNNNNNNLLLLLLLLLLLLLLLLLLLLLLLLLLNNNNbbbbbbsbbbbbbbbbbbbbbbbbbbbbbbbbb,N%%%%NNNNLNNNNN%%11NLNNN1LNNNNNLLLLLLLLLLLLLLLLLLLLLLLNLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLN"
var arabicTypes="nnnnnnNNr%%r,rNNmmmmmmmmmmmrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrmmmmmmmmmmmmmmmmmmmmmnnnnnnnnnn%nnrrrmrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrmmmmmmmnNmmmmmmrrmmNmmmmrr1111111111"
function charType(code){if(code<=0xf7){return lowTypes.charAt(code)}
else if(0x590<=code&&code<=0x5f4){return"R"}
else if(0x600<=code&&code<=0x6f9){return arabicTypes.charAt(code-0x600)}
else if(0x6ee<=code&&code<=0x8ac){return"r"}
else if(0x2000<=code&&code<=0x200b){return"w"}
else if(code==0x200c){return"b"}
else{return"L"}}
var bidiRE=/[\u0590-\u05f4\u0600-\u06ff\u0700-\u08ac]/
var isNeutral=/[stwN]/,isStrong=/[LRr]/,countsAsLeft=/[Lb1n]/,countsAsNum=/[1n]/
function BidiSpan(level,from,to){this.level=level
this.from=from;this.to=to}
return function(str,direction){var outerType=direction=="ltr"?"L":"R"
if(str.length==0||direction=="ltr"&&!bidiRE.test(str)){return false}
var len=str.length,types=[]
for(var i=0;i<len;++i)
{types.push(charType(str.charCodeAt(i)))}
for(var i$1=0,prev=outerType;i$1<len;++i$1){var type=types[i$1]
if(type=="m"){types[i$1]=prev}
else{prev=type}}
for(var i$2=0,cur=outerType;i$2<len;++i$2){var type$1=types[i$2]
if(type$1=="1"&&cur=="r"){types[i$2]="n"}
else if(isStrong.test(type$1)){cur=type$1;if(type$1=="r"){types[i$2]="R"}}}
for(var i$3=1,prev$1=types[0];i$3<len-1;++i$3){var type$2=types[i$3]
if(type$2=="+"&&prev$1=="1"&&types[i$3+1]=="1"){types[i$3]="1"}
else if(type$2==","&&prev$1==types[i$3+1]&&(prev$1=="1"||prev$1=="n")){types[i$3]=prev$1}
prev$1=type$2}
for(var i$4=0;i$4<len;++i$4){var type$3=types[i$4]
if(type$3==","){types[i$4]="N"}
else if(type$3=="%"){var end=(void 0)
for(end=i$4+1;end<len&&types[end]=="%";++end){}
var replace=(i$4&&types[i$4-1]=="!")||(end<len&&types[end]=="1")?"1":"N"
for(var j=i$4;j<end;++j){types[j]=replace}
i$4=end-1}}
for(var i$5=0,cur$1=outerType;i$5<len;++i$5){var type$4=types[i$5]
if(cur$1=="L"&&type$4=="1"){types[i$5]="L"}
else if(isStrong.test(type$4)){cur$1=type$4}}
for(var i$6=0;i$6<len;++i$6){if(isNeutral.test(types[i$6])){var end$1=(void 0)
for(end$1=i$6+1;end$1<len&&isNeutral.test(types[end$1]);++end$1){}
var before=(i$6?types[i$6-1]:outerType)=="L"
var after=(end$1<len?types[end$1]:outerType)=="L"
var replace$1=before==after?(before?"L":"R"):outerType
for(var j$1=i$6;j$1<end$1;++j$1){types[j$1]=replace$1}
i$6=end$1-1}}
var order=[],m
for(var i$7=0;i$7<len;){if(countsAsLeft.test(types[i$7])){var start=i$7
for(++i$7;i$7<len&&countsAsLeft.test(types[i$7]);++i$7){}
order.push(new BidiSpan(0,start,i$7))}else{var pos=i$7,at=order.length
for(++i$7;i$7<len&&types[i$7]!="L";++i$7){}
for(var j$2=pos;j$2<i$7;){if(countsAsNum.test(types[j$2])){if(pos<j$2){order.splice(at,0,new BidiSpan(1,pos,j$2))}
var nstart=j$2
for(++j$2;j$2<i$7&&countsAsNum.test(types[j$2]);++j$2){}
order.splice(at,0,new BidiSpan(2,nstart,j$2))
pos=j$2}else{++j$2}}
if(pos<i$7){order.splice(at,0,new BidiSpan(1,pos,i$7))}}}
if(order[0].level==1&&(m=str.match(/^\s+/))){order[0].from=m[0].length
order.unshift(new BidiSpan(0,0,m[0].length))}
if(lst(order).level==1&&(m=str.match(/\s+$/))){lst(order).to-=m[0].length
order.push(new BidiSpan(0,len-m[0].length,len))}
return direction=="rtl"?order.reverse():order}})()
function getOrder(line,direction){var order=line.order
if(order==null){order=line.order=bidiOrdering(line.text,direction)}
return order}
function moveCharLogically(line,ch,dir){var target=skipExtendingChars(line.text,ch+dir,dir)
return target<0||target>line.text.length?null:target}
function moveLogically(line,start,dir){var ch=moveCharLogically(line,start.ch,dir)
return ch==null?null:new Pos(start.line,ch,dir<0?"after":"before")}
function endOfLine(visually,cm,lineObj,lineNo,dir){if(visually){var order=getOrder(lineObj,cm.doc.direction)
if(order){var part=dir<0?lst(order):order[0]
var moveInStorageOrder=(dir<0)==(part.level==1)
var sticky=moveInStorageOrder?"after":"before"
var ch
if(part.level>0){var prep=prepareMeasureForLine(cm,lineObj)
ch=dir<0?lineObj.text.length-1:0
var targetTop=measureCharPrepared(cm,prep,ch).top
ch=findFirst(function(ch){return measureCharPrepared(cm,prep,ch).top==targetTop;},(dir<0)==(part.level==1)?part.from:part.to-1,ch)
if(sticky=="before"){ch=moveCharLogically(lineObj,ch,1,true)}}else{ch=dir<0?part.to:part.from}
return new Pos(lineNo,ch,sticky)}}
return new Pos(lineNo,dir<0?lineObj.text.length:0,dir<0?"before":"after")}
function moveVisually(cm,line,start,dir){var bidi=getOrder(line,cm.doc.direction)
if(!bidi){return moveLogically(line,start,dir)}
if(start.ch>=line.text.length){start.ch=line.text.length
start.sticky="before"}else if(start.ch<=0){start.ch=0
start.sticky="after"}
var partPos=getBidiPartAt(bidi,start.ch,start.sticky),part=bidi[partPos]
if(cm.doc.direction=="ltr"&&part.level%2==0&&(dir>0?part.to>start.ch:part.from<start.ch)){return moveLogically(line,start,dir)}
var mv=function(pos,dir){return moveCharLogically(line,pos instanceof Pos?pos.ch:pos,dir);}
var prep
var getWrappedLineExtent=function(ch){if(!cm.options.lineWrapping){return{begin:0,end:line.text.length}}
prep=prep||prepareMeasureForLine(cm,line)
return wrappedLineExtentChar(cm,line,prep,ch)}
var wrappedLineExtent=getWrappedLineExtent(start.sticky=="before"?mv(start,-1):start.ch)
if(cm.doc.direction=="rtl"||part.level==1){var moveInStorageOrder=(part.level==1)==(dir<0)
var ch=mv(start,moveInStorageOrder?1:-1)
if(ch!=null&&(!moveInStorageOrder?ch>=part.from&&ch>=wrappedLineExtent.begin:ch<=part.to&&ch<=wrappedLineExtent.end)){var sticky=moveInStorageOrder?"before":"after"
return new Pos(start.line,ch,sticky)}}
var searchInVisualLine=function(partPos,dir,wrappedLineExtent){var getRes=function(ch,moveInStorageOrder){return moveInStorageOrder?new Pos(start.line,mv(ch,1),"before"):new Pos(start.line,ch,"after");}
for(;partPos>=0&&partPos<bidi.length;partPos+=dir){var part=bidi[partPos]
var moveInStorageOrder=(dir>0)==(part.level!=1)
var ch=moveInStorageOrder?wrappedLineExtent.begin:mv(wrappedLineExtent.end,-1)
if(part.from<=ch&&ch<part.to){return getRes(ch,moveInStorageOrder)}
ch=moveInStorageOrder?part.from:mv(part.to,-1)
if(wrappedLineExtent.begin<=ch&&ch<wrappedLineExtent.end){return getRes(ch,moveInStorageOrder)}}}
var res=searchInVisualLine(partPos+dir,dir,wrappedLineExtent)
if(res){return res}
var nextCh=dir>0?wrappedLineExtent.end:mv(wrappedLineExtent.begin,-1)
if(nextCh!=null&&!(dir>0&&nextCh==line.text.length)){res=searchInVisualLine(dir>0?0:bidi.length-1,dir,getWrappedLineExtent(nextCh))
if(res){return res}}
return null}
var noHandlers=[]
var on=function(emitter,type,f){if(emitter.addEventListener){emitter.addEventListener(type,f,false)}else if(emitter.attachEvent){emitter.attachEvent("on"+type,f)}else{var map=emitter._handlers||(emitter._handlers={})
map[type]=(map[type]||noHandlers).concat(f)}}
function getHandlers(emitter,type){return emitter._handlers&&emitter._handlers[type]||noHandlers}
function off(emitter,type,f){if(emitter.removeEventListener){emitter.removeEventListener(type,f,false)}else if(emitter.detachEvent){emitter.detachEvent("on"+type,f)}else{var map=emitter._handlers,arr=map&&map[type]
if(arr){var index=indexOf(arr,f)
if(index>-1)
{map[type]=arr.slice(0,index).concat(arr.slice(index+1))}}}}
function signal(emitter,type){var handlers=getHandlers(emitter,type)
if(!handlers.length){return}
var args=Array.prototype.slice.call(arguments,2)
for(var i=0;i<handlers.length;++i){handlers[i].apply(null,args)}}
function signalDOMEvent(cm,e,override){if(typeof e=="string")
{e={type:e,preventDefault:function(){this.defaultPrevented=true}}}
signal(cm,override||e.type,cm,e)
return e_defaultPrevented(e)||e.codemirrorIgnore}
function signalCursorActivity(cm){var arr=cm._handlers&&cm._handlers.cursorActivity
if(!arr){return}
var set=cm.curOp.cursorActivityHandlers||(cm.curOp.cursorActivityHandlers=[])
for(var i=0;i<arr.length;++i){if(indexOf(set,arr[i])==-1)
{set.push(arr[i])}}}
function hasHandler(emitter,type){return getHandlers(emitter,type).length>0}
function eventMixin(ctor){ctor.prototype.on=function(type,f){on(this,type,f)}
ctor.prototype.off=function(type,f){off(this,type,f)}}
function e_preventDefault(e){if(e.preventDefault){e.preventDefault()}
else{e.returnValue=false}}
function e_stopPropagation(e){if(e.stopPropagation){e.stopPropagation()}
else{e.cancelBubble=true}}
function e_defaultPrevented(e){return e.defaultPrevented!=null?e.defaultPrevented:e.returnValue==false}
function e_stop(e){e_preventDefault(e);e_stopPropagation(e)}
function e_target(e){return e.target||e.srcElement}
function e_button(e){var b=e.which
if(b==null){if(e.button&1){b=1}
else if(e.button&2){b=3}
else if(e.button&4){b=2}}
if(mac&&e.ctrlKey&&b==1){b=3}
return b}
var dragAndDrop=function(){if(ie&&ie_version<9){return false}
var div=elt('div')
return"draggable"in div||"dragDrop"in div}()
var zwspSupported
function zeroWidthElement(measure){if(zwspSupported==null){var test=elt("span","\u200b")
removeChildrenAndAdd(measure,elt("span",[test,document.createTextNode("x")]))
if(measure.firstChild.offsetHeight!=0)
{zwspSupported=test.offsetWidth<=1&&test.offsetHeight>2&&!(ie&&ie_version<8)}}
var node=zwspSupported?elt("span","\u200b"):elt("span","\u00a0",null,"display: inline-block; width: 1px; margin-right: -1px")
node.setAttribute("cm-text","")
return node}
var badBidiRects
function hasBadBidiRects(measure){if(badBidiRects!=null){return badBidiRects}
var txt=removeChildrenAndAdd(measure,document.createTextNode("A\u062eA"))
var r0=range(txt,0,1).getBoundingClientRect()
var r1=range(txt,1,2).getBoundingClientRect()
removeChildren(measure)
if(!r0||r0.left==r0.right){return false}
return badBidiRects=(r1.right-r0.right<3)}
var splitLinesAuto="\n\nb".split(/\n/).length!=3?function(string){var pos=0,result=[],l=string.length
while(pos<=l){var nl=string.indexOf("\n",pos)
if(nl==-1){nl=string.length}
var line=string.slice(pos,string.charAt(nl-1)=="\r"?nl-1:nl)
var rt=line.indexOf("\r")
if(rt!=-1){result.push(line.slice(0,rt))
pos+=rt+1}else{result.push(line)
pos=nl+1}}
return result}:function(string){return string.split(/\r\n?|\n/);}
var hasSelection=window.getSelection?function(te){try{return te.selectionStart!=te.selectionEnd}
catch(e){return false}}:function(te){var range
try{range=te.ownerDocument.selection.createRange()}
catch(e){}
if(!range||range.parentElement()!=te){return false}
return range.compareEndPoints("StartToEnd",range)!=0}
var hasCopyEvent=(function(){var e=elt("div")
if("oncopy"in e){return true}
e.setAttribute("oncopy","return;")
return typeof e.oncopy=="function"})()
var badZoomedRects=null
function hasBadZoomedRects(measure){if(badZoomedRects!=null){return badZoomedRects}
var node=removeChildrenAndAdd(measure,elt("span","x"))
var normal=node.getBoundingClientRect()
var fromRange=range(node,0,1).getBoundingClientRect()
return badZoomedRects=Math.abs(normal.left-fromRange.left)>1}
var modes={};var mimeModes={};function defineMode(name,mode){if(arguments.length>2)
{mode.dependencies=Array.prototype.slice.call(arguments,2)}
modes[name]=mode}
function defineMIME(mime,spec){mimeModes[mime]=spec}
function resolveMode(spec){if(typeof spec=="string"&&mimeModes.hasOwnProperty(spec)){spec=mimeModes[spec]}else if(spec&&typeof spec.name=="string"&&mimeModes.hasOwnProperty(spec.name)){var found=mimeModes[spec.name]
if(typeof found=="string"){found={name:found}}
spec=createObj(found,spec)
spec.name=found.name}else if(typeof spec=="string"&&/^[\w\-]+\/[\w\-]+\+xml$/.test(spec)){return resolveMode("application/xml")}else if(typeof spec=="string"&&/^[\w\-]+\/[\w\-]+\+json$/.test(spec)){return resolveMode("application/json")}
if(typeof spec=="string"){return{name:spec}}
else{return spec||{name:"null"}}}
function getMode(options,spec){spec=resolveMode(spec)
var mfactory=modes[spec.name]
if(!mfactory){return getMode(options,"text/plain")}
var modeObj=mfactory(options,spec)
if(modeExtensions.hasOwnProperty(spec.name)){var exts=modeExtensions[spec.name]
for(var prop in exts){if(!exts.hasOwnProperty(prop)){continue}
if(modeObj.hasOwnProperty(prop)){modeObj["_"+prop]=modeObj[prop]}
modeObj[prop]=exts[prop]}}
modeObj.name=spec.name
if(spec.helperType){modeObj.helperType=spec.helperType}
if(spec.modeProps){for(var prop$1 in spec.modeProps)
{modeObj[prop$1]=spec.modeProps[prop$1]}}
return modeObj}
var modeExtensions={}
function extendMode(mode,properties){var exts=modeExtensions.hasOwnProperty(mode)?modeExtensions[mode]:(modeExtensions[mode]={})
copyObj(properties,exts)}
function copyState(mode,state){if(state===true){return state}
if(mode.copyState){return mode.copyState(state)}
var nstate={}
for(var n in state){var val=state[n]
if(val instanceof Array){val=val.concat([])}
nstate[n]=val}
return nstate}
function innerMode(mode,state){var info
while(mode.innerMode){info=mode.innerMode(state)
if(!info||info.mode==mode){break}
state=info.state
mode=info.mode}
return info||{mode:mode,state:state}}
function startState(mode,a1,a2){return mode.startState?mode.startState(a1,a2):true}
var StringStream=function StringStream(string,tabSize){this.pos=this.start=0
this.string=string
this.tabSize=tabSize||8
this.lastColumnPos=this.lastColumnValue=0
this.lineStart=0};StringStream.prototype.eol=function eol(){return this.pos>=this.string.length};StringStream.prototype.sol=function sol(){return this.pos==this.lineStart};StringStream.prototype.peek=function peek(){return this.string.charAt(this.pos)||undefined};StringStream.prototype.next=function next(){if(this.pos<this.string.length)
{return this.string.charAt(this.pos++)}};StringStream.prototype.eat=function eat(match){var ch=this.string.charAt(this.pos)
var ok
if(typeof match=="string"){ok=ch==match}
else{ok=ch&&(match.test?match.test(ch):match(ch))}
if(ok){++this.pos;return ch}};StringStream.prototype.eatWhile=function eatWhile(match){var start=this.pos
while(this.eat(match)){}
return this.pos>start};StringStream.prototype.eatSpace=function eatSpace(){var this$1=this;var start=this.pos
while(/[\s\u00a0]/.test(this.string.charAt(this.pos))){++this$1.pos}
return this.pos>start};StringStream.prototype.skipToEnd=function skipToEnd(){this.pos=this.string.length};StringStream.prototype.skipTo=function skipTo(ch){var found=this.string.indexOf(ch,this.pos)
if(found>-1){this.pos=found;return true}};StringStream.prototype.backUp=function backUp(n){this.pos-=n};StringStream.prototype.column=function column(){if(this.lastColumnPos<this.start){this.lastColumnValue=countColumn(this.string,this.start,this.tabSize,this.lastColumnPos,this.lastColumnValue)
this.lastColumnPos=this.start}
return this.lastColumnValue-(this.lineStart?countColumn(this.string,this.lineStart,this.tabSize):0)};StringStream.prototype.indentation=function indentation(){return countColumn(this.string,null,this.tabSize)-
(this.lineStart?countColumn(this.string,this.lineStart,this.tabSize):0)};StringStream.prototype.match=function match(pattern,consume,caseInsensitive){if(typeof pattern=="string"){var cased=function(str){return caseInsensitive?str.toLowerCase():str;}
var substr=this.string.substr(this.pos,pattern.length)
if(cased(substr)==cased(pattern)){if(consume!==false){this.pos+=pattern.length}
return true}}else{var match=this.string.slice(this.pos).match(pattern)
if(match&&match.index>0){return null}
if(match&&consume!==false){this.pos+=match[0].length}
return match}};StringStream.prototype.current=function current(){return this.string.slice(this.start,this.pos)};StringStream.prototype.hideFirstChars=function hideFirstChars(n,inner){this.lineStart+=n
try{return inner()}
finally{this.lineStart-=n}};function highlightLine(cm,line,state,forceToEnd){var st=[cm.state.modeGen],lineClasses={}
runMode(cm,line.text,cm.doc.mode,state,function(end,style){return st.push(end,style);},lineClasses,forceToEnd)
var loop=function(o){var overlay=cm.state.overlays[o],i=1,at=0
runMode(cm,line.text,overlay.mode,true,function(end,style){var start=i
while(at<end){var i_end=st[i]
if(i_end>end)
{st.splice(i,1,end,st[i+1],i_end)}
i+=2
at=Math.min(end,i_end)}
if(!style){return}
if(overlay.opaque){st.splice(start,i-start,end,"overlay "+style)
i=start+2}else{for(;start<i;start+=2){var cur=st[start+1]
st[start+1]=(cur?cur+" ":"")+"overlay "+style}}},lineClasses)};for(var o=0;o<cm.state.overlays.length;++o)loop(o);return{styles:st,classes:lineClasses.bgClass||lineClasses.textClass?lineClasses:null}}
function getLineStyles(cm,line,updateFrontier){if(!line.styles||line.styles[0]!=cm.state.modeGen){var state=getStateBefore(cm,lineNo(line))
var result=highlightLine(cm,line,line.text.length>cm.options.maxHighlightLength?copyState(cm.doc.mode,state):state)
line.stateAfter=state
line.styles=result.styles
if(result.classes){line.styleClasses=result.classes}
else if(line.styleClasses){line.styleClasses=null}
if(updateFrontier===cm.doc.frontier){cm.doc.frontier++}}
return line.styles}
function getStateBefore(cm,n,precise){var doc=cm.doc,display=cm.display
if(!doc.mode.startState){return true}
var pos=findStartLine(cm,n,precise),state=pos>doc.first&&getLine(doc,pos-1).stateAfter
if(!state){state=startState(doc.mode)}
else{state=copyState(doc.mode,state)}
doc.iter(pos,n,function(line){processLine(cm,line.text,state)
var save=pos==n-1||pos%5==0||pos>=display.viewFrom&&pos<display.viewTo
line.stateAfter=save?copyState(doc.mode,state):null
++pos})
if(precise){doc.frontier=pos}
return state}
function processLine(cm,text,state,startAt){var mode=cm.doc.mode
var stream=new StringStream(text,cm.options.tabSize)
stream.start=stream.pos=startAt||0
if(text==""){callBlankLine(mode,state)}
while(!stream.eol()){readToken(mode,stream,state)
stream.start=stream.pos}}
function callBlankLine(mode,state){if(mode.blankLine){return mode.blankLine(state)}
if(!mode.innerMode){return}
var inner=innerMode(mode,state)
if(inner.mode.blankLine){return inner.mode.blankLine(inner.state)}}
function readToken(mode,stream,state,inner){for(var i=0;i<10;i++){if(inner){inner[0]=innerMode(mode,state).mode}
var style=mode.token(stream,state)
if(stream.pos>stream.start){return style}}
throw new Error("Mode "+mode.name+" failed to advance stream.")}
function takeToken(cm,pos,precise,asArray){var getObj=function(copy){return({start:stream.start,end:stream.pos,string:stream.current(),type:style||null,state:copy?copyState(doc.mode,state):state});}
var doc=cm.doc,mode=doc.mode,style
pos=clipPos(doc,pos)
var line=getLine(doc,pos.line),state=getStateBefore(cm,pos.line,precise)
var stream=new StringStream(line.text,cm.options.tabSize),tokens
if(asArray){tokens=[]}
while((asArray||stream.pos<pos.ch)&&!stream.eol()){stream.start=stream.pos
style=readToken(mode,stream,state)
if(asArray){tokens.push(getObj(true))}}
return asArray?tokens:getObj()}
function extractLineClasses(type,output){if(type){for(;;){var lineClass=type.match(/(?:^|\s+)line-(background-)?(\S+)/)
if(!lineClass){break}
type=type.slice(0,lineClass.index)+type.slice(lineClass.index+lineClass[0].length)
var prop=lineClass[1]?"bgClass":"textClass"
if(output[prop]==null)
{output[prop]=lineClass[2]}
else if(!(new RegExp("(?:^|\s)"+lineClass[2]+"(?:$|\s)")).test(output[prop]))
{output[prop]+=" "+lineClass[2]}}}
return type}
function runMode(cm,text,mode,state,f,lineClasses,forceToEnd){var flattenSpans=mode.flattenSpans
if(flattenSpans==null){flattenSpans=cm.options.flattenSpans}
var curStart=0,curStyle=null
var stream=new StringStream(text,cm.options.tabSize),style
var inner=cm.options.addModeClass&&[null]
if(text==""){extractLineClasses(callBlankLine(mode,state),lineClasses)}
while(!stream.eol()){if(stream.pos>cm.options.maxHighlightLength){flattenSpans=false
if(forceToEnd){processLine(cm,text,state,stream.pos)}
stream.pos=text.length
style=null}else{style=extractLineClasses(readToken(mode,stream,state,inner),lineClasses)}
if(inner){var mName=inner[0].name
if(mName){style="m-"+(style?mName+" "+style:mName)}}
if(!flattenSpans||curStyle!=style){while(curStart<stream.start){curStart=Math.min(stream.start,curStart+5000)
f(curStart,curStyle)}
curStyle=style}
stream.start=stream.pos}
while(curStart<stream.pos){var pos=Math.min(stream.pos,curStart+5000)
f(pos,curStyle)
curStart=pos}}
function findStartLine(cm,n,precise){var minindent,minline,doc=cm.doc
var lim=precise?-1:n-(cm.doc.mode.innerMode?1000:100)
for(var search=n;search>lim;--search){if(search<=doc.first){return doc.first}
var line=getLine(doc,search-1)
if(line.stateAfter&&(!precise||search<=doc.frontier)){return search}
var indented=countColumn(line.text,null,cm.options.tabSize)
if(minline==null||minindent>indented){minline=search-1
minindent=indented}}
return minline}
var Line=function Line(text,markedSpans,estimateHeight){this.text=text
attachMarkedSpans(this,markedSpans)
this.height=estimateHeight?estimateHeight(this):1};Line.prototype.lineNo=function lineNo$1(){return lineNo(this)};eventMixin(Line)
function updateLine(line,text,markedSpans,estimateHeight){line.text=text
if(line.stateAfter){line.stateAfter=null}
if(line.styles){line.styles=null}
if(line.order!=null){line.order=null}
detachMarkedSpans(line)
attachMarkedSpans(line,markedSpans)
var estHeight=estimateHeight?estimateHeight(line):1
if(estHeight!=line.height){updateLineHeight(line,estHeight)}}
function cleanUpLine(line){line.parent=null
detachMarkedSpans(line)}
var styleToClassCache={};var styleToClassCacheWithMode={};function interpretTokenStyle(style,options){if(!style||/^\s*$/.test(style)){return null}
var cache=options.addModeClass?styleToClassCacheWithMode:styleToClassCache
return cache[style]||(cache[style]=style.replace(/\S+/g,"cm-$&"))}
function buildLineContent(cm,lineView){var content=eltP("span",null,null,webkit?"padding-right: .1px":null)
var builder={pre:eltP("pre",[content],"CodeMirror-line"),content:content,col:0,pos:0,cm:cm,trailingSpace:false,splitSpaces:(ie||webkit)&&cm.getOption("lineWrapping")}
lineView.measure={}
for(var i=0;i<=(lineView.rest?lineView.rest.length:0);i++){var line=i?lineView.rest[i-1]:lineView.line,order=(void 0)
builder.pos=0
builder.addToken=buildToken
if(hasBadBidiRects(cm.display.measure)&&(order=getOrder(line,cm.doc.direction)))
{builder.addToken=buildTokenBadBidi(builder.addToken,order)}
builder.map=[]
var allowFrontierUpdate=lineView!=cm.display.externalMeasured&&lineNo(line)
insertLineContent(line,builder,getLineStyles(cm,line,allowFrontierUpdate))
if(line.styleClasses){if(line.styleClasses.bgClass)
{builder.bgClass=joinClasses(line.styleClasses.bgClass,builder.bgClass||"")}
if(line.styleClasses.textClass)
{builder.textClass=joinClasses(line.styleClasses.textClass,builder.textClass||"")}}
if(builder.map.length==0)
{builder.map.push(0,0,builder.content.appendChild(zeroWidthElement(cm.display.measure)))}
if(i==0){lineView.measure.map=builder.map
lineView.measure.cache={}}else{;(lineView.measure.maps||(lineView.measure.maps=[])).push(builder.map);(lineView.measure.caches||(lineView.measure.caches=[])).push({})}}
if(webkit){var last=builder.content.lastChild
if(/\bcm-tab\b/.test(last.className)||(last.querySelector&&last.querySelector(".cm-tab")))
{builder.content.className="cm-tab-wrap-hack"}}
signal(cm,"renderLine",cm,lineView.line,builder.pre)
if(builder.pre.className)
{builder.textClass=joinClasses(builder.pre.className,builder.textClass||"")}
return builder}
function defaultSpecialCharPlaceholder(ch){var token=elt("span","\u2022","cm-invalidchar")
token.title="\\u"+ch.charCodeAt(0).toString(16)
token.setAttribute("aria-label",token.title)
return token}
function buildToken(builder,text,style,startStyle,endStyle,title,css){if(!text){return}
var displayText=builder.splitSpaces?splitSpaces(text,builder.trailingSpace):text
var special=builder.cm.state.specialChars,mustWrap=false
var content
if(!special.test(text)){builder.col+=text.length
content=document.createTextNode(displayText)
builder.map.push(builder.pos,builder.pos+text.length,content)
if(ie&&ie_version<9){mustWrap=true}
builder.pos+=text.length}else{content=document.createDocumentFragment()
var pos=0
while(true){special.lastIndex=pos
var m=special.exec(text)
var skipped=m?m.index-pos:text.length-pos
if(skipped){var txt=document.createTextNode(displayText.slice(pos,pos+skipped))
if(ie&&ie_version<9){content.appendChild(elt("span",[txt]))}
else{content.appendChild(txt)}
builder.map.push(builder.pos,builder.pos+skipped,txt)
builder.col+=skipped
builder.pos+=skipped}
if(!m){break}
pos+=skipped+1
var txt$1=(void 0)
if(m[0]=="\t"){var tabSize=builder.cm.options.tabSize,tabWidth=tabSize-builder.col%tabSize
txt$1=content.appendChild(elt("span",spaceStr(tabWidth),"cm-tab"))
txt$1.setAttribute("role","presentation")
txt$1.setAttribute("cm-text","\t")
builder.col+=tabWidth}else if(m[0]=="\r"||m[0]=="\n"){txt$1=content.appendChild(elt("span",m[0]=="\r"?"\u240d":"\u2424","cm-invalidchar"))
txt$1.setAttribute("cm-text",m[0])
builder.col+=1}else{txt$1=builder.cm.options.specialCharPlaceholder(m[0])
txt$1.setAttribute("cm-text",m[0])
if(ie&&ie_version<9){content.appendChild(elt("span",[txt$1]))}
else{content.appendChild(txt$1)}
builder.col+=1}
builder.map.push(builder.pos,builder.pos+1,txt$1)
builder.pos++}}
builder.trailingSpace=displayText.charCodeAt(text.length-1)==32
if(style||startStyle||endStyle||mustWrap||css){var fullStyle=style||""
if(startStyle){fullStyle+=startStyle}
if(endStyle){fullStyle+=endStyle}
var token=elt("span",[content],fullStyle,css)
if(title){token.title=title}
return builder.content.appendChild(token)}
builder.content.appendChild(content)}
function splitSpaces(text,trailingBefore){if(text.length>1&&!/  /.test(text)){return text}
var spaceBefore=trailingBefore,result=""
for(var i=0;i<text.length;i++){var ch=text.charAt(i)
if(ch==" "&&spaceBefore&&(i==text.length-1||text.charCodeAt(i+1)==32))
{ch="\u00a0"}
result+=ch
spaceBefore=ch==" "}
return result}
function buildTokenBadBidi(inner,order){return function(builder,text,style,startStyle,endStyle,title,css){style=style?style+" cm-force-border":"cm-force-border"
var start=builder.pos,end=start+text.length
for(;;){var part=(void 0)
for(var i=0;i<order.length;i++){part=order[i]
if(part.to>start&&part.from<=start){break}}
if(part.to>=end){return inner(builder,text,style,startStyle,endStyle,title,css)}
inner(builder,text.slice(0,part.to-start),style,startStyle,null,title,css)
startStyle=null
text=text.slice(part.to-start)
start=part.to}}}
function buildCollapsedSpan(builder,size,marker,ignoreWidget){var widget=!ignoreWidget&&marker.widgetNode
if(widget){builder.map.push(builder.pos,builder.pos+size,widget)}
if(!ignoreWidget&&builder.cm.display.input.needsContentAttribute){if(!widget)
{widget=builder.content.appendChild(document.createElement("span"))}
widget.setAttribute("cm-marker",marker.id)}
if(widget){builder.cm.display.input.setUneditable(widget)
builder.content.appendChild(widget)}
builder.pos+=size
builder.trailingSpace=false}
function insertLineContent(line,builder,styles){var spans=line.markedSpans,allText=line.text,at=0
if(!spans){for(var i$1=1;i$1<styles.length;i$1+=2)
{builder.addToken(builder,allText.slice(at,at=styles[i$1]),interpretTokenStyle(styles[i$1+1],builder.cm.options))}
return}
var len=allText.length,pos=0,i=1,text="",style,css
var nextChange=0,spanStyle,spanEndStyle,spanStartStyle,title,collapsed
for(;;){if(nextChange==pos){spanStyle=spanEndStyle=spanStartStyle=title=css=""
collapsed=null;nextChange=Infinity
var foundBookmarks=[],endStyles=(void 0)
for(var j=0;j<spans.length;++j){var sp=spans[j],m=sp.marker
if(m.type=="bookmark"&&sp.from==pos&&m.widgetNode){foundBookmarks.push(m)}else if(sp.from<=pos&&(sp.to==null||sp.to>pos||m.collapsed&&sp.to==pos&&sp.from==pos)){if(sp.to!=null&&sp.to!=pos&&nextChange>sp.to){nextChange=sp.to
spanEndStyle=""}
if(m.className){spanStyle+=" "+m.className}
if(m.css){css=(css?css+";":"")+m.css}
if(m.startStyle&&sp.from==pos){spanStartStyle+=" "+m.startStyle}
if(m.endStyle&&sp.to==nextChange){(endStyles||(endStyles=[])).push(m.endStyle,sp.to)}
if(m.title&&!title){title=m.title}
if(m.collapsed&&(!collapsed||compareCollapsedMarkers(collapsed.marker,m)<0))
{collapsed=sp}}else if(sp.from>pos&&nextChange>sp.from){nextChange=sp.from}}
if(endStyles){for(var j$1=0;j$1<endStyles.length;j$1+=2)
{if(endStyles[j$1+1]==nextChange){spanEndStyle+=" "+endStyles[j$1]}}}
if(!collapsed||collapsed.from==pos){for(var j$2=0;j$2<foundBookmarks.length;++j$2)
{buildCollapsedSpan(builder,0,foundBookmarks[j$2])}}
if(collapsed&&(collapsed.from||0)==pos){buildCollapsedSpan(builder,(collapsed.to==null?len+1:collapsed.to)-pos,collapsed.marker,collapsed.from==null)
if(collapsed.to==null){return}
if(collapsed.to==pos){collapsed=false}}}
if(pos>=len){break}
var upto=Math.min(len,nextChange)
while(true){if(text){var end=pos+text.length
if(!collapsed){var tokenText=end>upto?text.slice(0,upto-pos):text
builder.addToken(builder,tokenText,style?style+spanStyle:spanStyle,spanStartStyle,pos+tokenText.length==nextChange?spanEndStyle:"",title,css)}
if(end>=upto){text=text.slice(upto-pos);pos=upto;break}
pos=end
spanStartStyle=""}
text=allText.slice(at,at=styles[i++])
style=interpretTokenStyle(styles[i++],builder.cm.options)}}}
function LineView(doc,line,lineN){this.line=line
this.rest=visualLineContinued(line)
this.size=this.rest?lineNo(lst(this.rest))-lineN+1:1
this.node=this.text=null
this.hidden=lineIsHidden(doc,line)}
function buildViewArray(cm,from,to){var array=[],nextPos
for(var pos=from;pos<to;pos=nextPos){var view=new LineView(cm.doc,getLine(cm.doc,pos),pos)
nextPos=pos+view.size
array.push(view)}
return array}
var operationGroup=null
function pushOperation(op){if(operationGroup){operationGroup.ops.push(op)}else{op.ownsGroup=operationGroup={ops:[op],delayedCallbacks:[]}}}
function fireCallbacksForOps(group){var callbacks=group.delayedCallbacks,i=0
do{for(;i<callbacks.length;i++)
{callbacks[i].call(null)}
for(var j=0;j<group.ops.length;j++){var op=group.ops[j]
if(op.cursorActivityHandlers)
{while(op.cursorActivityCalled<op.cursorActivityHandlers.length)
{op.cursorActivityHandlers[op.cursorActivityCalled++].call(null,op.cm)}}}}while(i<callbacks.length)}
function finishOperation(op,endCb){var group=op.ownsGroup
if(!group){return}
try{fireCallbacksForOps(group)}
finally{operationGroup=null
endCb(group)}}
var orphanDelayedCallbacks=null
function signalLater(emitter,type){var arr=getHandlers(emitter,type)
if(!arr.length){return}
var args=Array.prototype.slice.call(arguments,2),list
if(operationGroup){list=operationGroup.delayedCallbacks}else if(orphanDelayedCallbacks){list=orphanDelayedCallbacks}else{list=orphanDelayedCallbacks=[]
setTimeout(fireOrphanDelayed,0)}
var loop=function(i){list.push(function(){return arr[i].apply(null,args);})};for(var i=0;i<arr.length;++i)
loop(i);}
function fireOrphanDelayed(){var delayed=orphanDelayedCallbacks
orphanDelayedCallbacks=null
for(var i=0;i<delayed.length;++i){delayed[i]()}}
function updateLineForChanges(cm,lineView,lineN,dims){for(var j=0;j<lineView.changes.length;j++){var type=lineView.changes[j]
if(type=="text"){updateLineText(cm,lineView)}
else if(type=="gutter"){updateLineGutter(cm,lineView,lineN,dims)}
else if(type=="class"){updateLineClasses(cm,lineView)}
else if(type=="widget"){updateLineWidgets(cm,lineView,dims)}}
lineView.changes=null}
function ensureLineWrapped(lineView){if(lineView.node==lineView.text){lineView.node=elt("div",null,null,"position: relative")
if(lineView.text.parentNode)
{lineView.text.parentNode.replaceChild(lineView.node,lineView.text)}
lineView.node.appendChild(lineView.text)
if(ie&&ie_version<8){lineView.node.style.zIndex=2}}
return lineView.node}
function updateLineBackground(cm,lineView){var cls=lineView.bgClass?lineView.bgClass+" "+(lineView.line.bgClass||""):lineView.line.bgClass
if(cls){cls+=" CodeMirror-linebackground"}
if(lineView.background){if(cls){lineView.background.className=cls}
else{lineView.background.parentNode.removeChild(lineView.background);lineView.background=null}}else if(cls){var wrap=ensureLineWrapped(lineView)
lineView.background=wrap.insertBefore(elt("div",null,cls),wrap.firstChild)
cm.display.input.setUneditable(lineView.background)}}
function getLineContent(cm,lineView){var ext=cm.display.externalMeasured
if(ext&&ext.line==lineView.line){cm.display.externalMeasured=null
lineView.measure=ext.measure
return ext.built}
return buildLineContent(cm,lineView)}
function updateLineText(cm,lineView){var cls=lineView.text.className
var built=getLineContent(cm,lineView)
if(lineView.text==lineView.node){lineView.node=built.pre}
lineView.text.parentNode.replaceChild(built.pre,lineView.text)
lineView.text=built.pre
if(built.bgClass!=lineView.bgClass||built.textClass!=lineView.textClass){lineView.bgClass=built.bgClass
lineView.textClass=built.textClass
updateLineClasses(cm,lineView)}else if(cls){lineView.text.className=cls}}
function updateLineClasses(cm,lineView){updateLineBackground(cm,lineView)
if(lineView.line.wrapClass)
{ensureLineWrapped(lineView).className=lineView.line.wrapClass}
else if(lineView.node!=lineView.text)
{lineView.node.className=""}
var textClass=lineView.textClass?lineView.textClass+" "+(lineView.line.textClass||""):lineView.line.textClass
lineView.text.className=textClass||""}
function updateLineGutter(cm,lineView,lineN,dims){if(lineView.gutter){lineView.node.removeChild(lineView.gutter)
lineView.gutter=null}
if(lineView.gutterBackground){lineView.node.removeChild(lineView.gutterBackground)
lineView.gutterBackground=null}
if(lineView.line.gutterClass){var wrap=ensureLineWrapped(lineView)
lineView.gutterBackground=elt("div",null,"CodeMirror-gutter-background "+lineView.line.gutterClass,("left: "+(cm.options.fixedGutter?dims.fixedPos:-dims.gutterTotalWidth)+"px; width: "+(dims.gutterTotalWidth)+"px"))
cm.display.input.setUneditable(lineView.gutterBackground)
wrap.insertBefore(lineView.gutterBackground,lineView.text)}
var markers=lineView.line.gutterMarkers
if(cm.options.lineNumbers||markers){var wrap$1=ensureLineWrapped(lineView)
var gutterWrap=lineView.gutter=elt("div",null,"CodeMirror-gutter-wrapper",("left: "+(cm.options.fixedGutter?dims.fixedPos:-dims.gutterTotalWidth)+"px"))
cm.display.input.setUneditable(gutterWrap)
wrap$1.insertBefore(gutterWrap,lineView.text)
if(lineView.line.gutterClass)
{gutterWrap.className+=" "+lineView.line.gutterClass}
if(cm.options.lineNumbers&&(!markers||!markers["CodeMirror-linenumbers"]))
{lineView.lineNumber=gutterWrap.appendChild(elt("div",lineNumberFor(cm.options,lineN),"CodeMirror-linenumber CodeMirror-gutter-elt",("left: "+(dims.gutterLeft["CodeMirror-linenumbers"])+"px; width: "+(cm.display.lineNumInnerWidth)+"px")))}
if(markers){for(var k=0;k<cm.options.gutters.length;++k){var id=cm.options.gutters[k],found=markers.hasOwnProperty(id)&&markers[id]
if(found)
{gutterWrap.appendChild(elt("div",[found],"CodeMirror-gutter-elt",("left: "+(dims.gutterLeft[id])+"px; width: "+(dims.gutterWidth[id])+"px")))}}}}}
function updateLineWidgets(cm,lineView,dims){if(lineView.alignable){lineView.alignable=null}
for(var node=lineView.node.firstChild,next=(void 0);node;node=next){next=node.nextSibling
if(node.className=="CodeMirror-linewidget")
{lineView.node.removeChild(node)}}
insertLineWidgets(cm,lineView,dims)}
function buildLineElement(cm,lineView,lineN,dims){var built=getLineContent(cm,lineView)
lineView.text=lineView.node=built.pre
if(built.bgClass){lineView.bgClass=built.bgClass}
if(built.textClass){lineView.textClass=built.textClass}
updateLineClasses(cm,lineView)
updateLineGutter(cm,lineView,lineN,dims)
insertLineWidgets(cm,lineView,dims)
return lineView.node}
function insertLineWidgets(cm,lineView,dims){insertLineWidgetsFor(cm,lineView.line,lineView,dims,true)
if(lineView.rest){for(var i=0;i<lineView.rest.length;i++)
{insertLineWidgetsFor(cm,lineView.rest[i],lineView,dims,false)}}}
function insertLineWidgetsFor(cm,line,lineView,dims,allowAbove){if(!line.widgets){return}
var wrap=ensureLineWrapped(lineView)
for(var i=0,ws=line.widgets;i<ws.length;++i){var widget=ws[i],node=elt("div",[widget.node],"CodeMirror-linewidget")
if(!widget.handleMouseEvents){node.setAttribute("cm-ignore-events","true")}
positionLineWidget(widget,node,lineView,dims)
cm.display.input.setUneditable(node)
if(allowAbove&&widget.above)
{wrap.insertBefore(node,lineView.gutter||lineView.text)}
else
{wrap.appendChild(node)}
signalLater(widget,"redraw")}}
function positionLineWidget(widget,node,lineView,dims){if(widget.noHScroll){;(lineView.alignable||(lineView.alignable=[])).push(node)
var width=dims.wrapperWidth
node.style.left=dims.fixedPos+"px"
if(!widget.coverGutter){width-=dims.gutterTotalWidth
node.style.paddingLeft=dims.gutterTotalWidth+"px"}
node.style.width=width+"px"}
if(widget.coverGutter){node.style.zIndex=5
node.style.position="relative"
if(!widget.noHScroll){node.style.marginLeft=-dims.gutterTotalWidth+"px"}}}
function widgetHeight(widget){if(widget.height!=null){return widget.height}
var cm=widget.doc.cm
if(!cm){return 0}
if(!contains(document.body,widget.node)){var parentStyle="position: relative;"
if(widget.coverGutter)
{parentStyle+="margin-left: -"+cm.display.gutters.offsetWidth+"px;"}
if(widget.noHScroll)
{parentStyle+="width: "+cm.display.wrapper.clientWidth+"px;"}
removeChildrenAndAdd(cm.display.measure,elt("div",[widget.node],null,parentStyle))}
return widget.height=widget.node.parentNode.offsetHeight}
function eventInWidget(display,e){for(var n=e_target(e);n!=display.wrapper;n=n.parentNode){if(!n||(n.nodeType==1&&n.getAttribute("cm-ignore-events")=="true")||(n.parentNode==display.sizer&&n!=display.mover))
{return true}}}
function paddingTop(display){return display.lineSpace.offsetTop}
function paddingVert(display){return display.mover.offsetHeight-display.lineSpace.offsetHeight}
function paddingH(display){if(display.cachedPaddingH){return display.cachedPaddingH}
var e=removeChildrenAndAdd(display.measure,elt("pre","x"))
var style=window.getComputedStyle?window.getComputedStyle(e):e.currentStyle
var data={left:parseInt(style.paddingLeft),right:parseInt(style.paddingRight)}
if(!isNaN(data.left)&&!isNaN(data.right)){display.cachedPaddingH=data}
return data}
function scrollGap(cm){return scrollerGap-cm.display.nativeBarWidth}
function displayWidth(cm){return cm.display.scroller.clientWidth-scrollGap(cm)-cm.display.barWidth}
function displayHeight(cm){return cm.display.scroller.clientHeight-scrollGap(cm)-cm.display.barHeight}
function ensureLineHeights(cm,lineView,rect){var wrapping=cm.options.lineWrapping
var curWidth=wrapping&&displayWidth(cm)
if(!lineView.measure.heights||wrapping&&lineView.measure.width!=curWidth){var heights=lineView.measure.heights=[]
if(wrapping){lineView.measure.width=curWidth
var rects=lineView.text.firstChild.getClientRects()
for(var i=0;i<rects.length-1;i++){var cur=rects[i],next=rects[i+1]
if(Math.abs(cur.bottom-next.bottom)>2)
{heights.push((cur.bottom+next.top)/2-rect.top)}}}
heights.push(rect.bottom-rect.top)}}
function mapFromLineView(lineView,line,lineN){if(lineView.line==line)
{return{map:lineView.measure.map,cache:lineView.measure.cache}}
for(var i=0;i<lineView.rest.length;i++)
{if(lineView.rest[i]==line)
{return{map:lineView.measure.maps[i],cache:lineView.measure.caches[i]}}}
for(var i$1=0;i$1<lineView.rest.length;i$1++)
{if(lineNo(lineView.rest[i$1])>lineN)
{return{map:lineView.measure.maps[i$1],cache:lineView.measure.caches[i$1],before:true}}}}
function updateExternalMeasurement(cm,line){line=visualLine(line)
var lineN=lineNo(line)
var view=cm.display.externalMeasured=new LineView(cm.doc,line,lineN)
view.lineN=lineN
var built=view.built=buildLineContent(cm,view)
view.text=built.pre
removeChildrenAndAdd(cm.display.lineMeasure,built.pre)
return view}
function measureChar(cm,line,ch,bias){return measureCharPrepared(cm,prepareMeasureForLine(cm,line),ch,bias)}
function findViewForLine(cm,lineN){if(lineN>=cm.display.viewFrom&&lineN<cm.display.viewTo)
{return cm.display.view[findViewIndex(cm,lineN)]}
var ext=cm.display.externalMeasured
if(ext&&lineN>=ext.lineN&&lineN<ext.lineN+ext.size)
{return ext}}
function prepareMeasureForLine(cm,line){var lineN=lineNo(line)
var view=findViewForLine(cm,lineN)
if(view&&!view.text){view=null}else if(view&&view.changes){updateLineForChanges(cm,view,lineN,getDimensions(cm))
cm.curOp.forceUpdate=true}
if(!view)
{view=updateExternalMeasurement(cm,line)}
var info=mapFromLineView(view,line,lineN)
return{line:line,view:view,rect:null,map:info.map,cache:info.cache,before:info.before,hasHeights:false}}
function measureCharPrepared(cm,prepared,ch,bias,varHeight){if(prepared.before){ch=-1}
var key=ch+(bias||""),found
if(prepared.cache.hasOwnProperty(key)){found=prepared.cache[key]}else{if(!prepared.rect)
{prepared.rect=prepared.view.text.getBoundingClientRect()}
if(!prepared.hasHeights){ensureLineHeights(cm,prepared.view,prepared.rect)
prepared.hasHeights=true}
found=measureCharInner(cm,prepared,ch,bias)
if(!found.bogus){prepared.cache[key]=found}}
return{left:found.left,right:found.right,top:varHeight?found.rtop:found.top,bottom:varHeight?found.rbottom:found.bottom}}
var nullRect={left:0,right:0,top:0,bottom:0}
function nodeAndOffsetInLineMap(map,ch,bias){var node,start,end,collapse,mStart,mEnd
for(var i=0;i<map.length;i+=3){mStart=map[i]
mEnd=map[i+1]
if(ch<mStart){start=0;end=1
collapse="left"}else if(ch<mEnd){start=ch-mStart
end=start+1}else if(i==map.length-3||ch==mEnd&&map[i+3]>ch){end=mEnd-mStart
start=end-1
if(ch>=mEnd){collapse="right"}}
if(start!=null){node=map[i+2]
if(mStart==mEnd&&bias==(node.insertLeft?"left":"right"))
{collapse=bias}
if(bias=="left"&&start==0)
{while(i&&map[i-2]==map[i-3]&&map[i-1].insertLeft){node=map[(i-=3)+2]
collapse="left"}}
if(bias=="right"&&start==mEnd-mStart)
{while(i<map.length-3&&map[i+3]==map[i+4]&&!map[i+5].insertLeft){node=map[(i+=3)+2]
collapse="right"}}
break}}
return{node:node,start:start,end:end,collapse:collapse,coverStart:mStart,coverEnd:mEnd}}
function getUsefulRect(rects,bias){var rect=nullRect
if(bias=="left"){for(var i=0;i<rects.length;i++){if((rect=rects[i]).left!=rect.right){break}}}else{for(var i$1=rects.length-1;i$1>=0;i$1--){if((rect=rects[i$1]).left!=rect.right){break}}}
return rect}
function measureCharInner(cm,prepared,ch,bias){var place=nodeAndOffsetInLineMap(prepared.map,ch,bias)
var node=place.node,start=place.start,end=place.end,collapse=place.collapse
var rect
if(node.nodeType==3){for(var i$1=0;i$1<4;i$1++){while(start&&isExtendingChar(prepared.line.text.charAt(place.coverStart+start))){--start}
while(place.coverStart+end<place.coverEnd&&isExtendingChar(prepared.line.text.charAt(place.coverStart+end))){++end}
if(ie&&ie_version<9&&start==0&&end==place.coverEnd-place.coverStart)
{rect=node.parentNode.getBoundingClientRect()}
else
{rect=getUsefulRect(range(node,start,end).getClientRects(),bias)}
if(rect.left||rect.right||start==0){break}
end=start
start=start-1
collapse="right"}
if(ie&&ie_version<11){rect=maybeUpdateRectForZooming(cm.display.measure,rect)}}else{if(start>0){collapse=bias="right"}
var rects
if(cm.options.lineWrapping&&(rects=node.getClientRects()).length>1)
{rect=rects[bias=="right"?rects.length-1:0]}
else
{rect=node.getBoundingClientRect()}}
if(ie&&ie_version<9&&!start&&(!rect||!rect.left&&!rect.right)){var rSpan=node.parentNode.getClientRects()[0]
if(rSpan)
{rect={left:rSpan.left,right:rSpan.left+charWidth(cm.display),top:rSpan.top,bottom:rSpan.bottom}}
else
{rect=nullRect}}
var rtop=rect.top-prepared.rect.top,rbot=rect.bottom-prepared.rect.top
var mid=(rtop+rbot)/2
var heights=prepared.view.measure.heights
var i=0
for(;i<heights.length-1;i++)
{if(mid<heights[i]){break}}
var top=i?heights[i-1]:0,bot=heights[i]
var result={left:(collapse=="right"?rect.right:rect.left)-prepared.rect.left,right:(collapse=="left"?rect.left:rect.right)-prepared.rect.left,top:top,bottom:bot}
if(!rect.left&&!rect.right){result.bogus=true}
if(!cm.options.singleCursorHeightPerLine){result.rtop=rtop;result.rbottom=rbot}
return result}
function maybeUpdateRectForZooming(measure,rect){if(!window.screen||screen.logicalXDPI==null||screen.logicalXDPI==screen.deviceXDPI||!hasBadZoomedRects(measure))
{return rect}
var scaleX=screen.logicalXDPI/screen.deviceXDPI
var scaleY=screen.logicalYDPI/screen.deviceYDPI
return{left:rect.left*scaleX,right:rect.right*scaleX,top:rect.top*scaleY,bottom:rect.bottom*scaleY}}
function clearLineMeasurementCacheFor(lineView){if(lineView.measure){lineView.measure.cache={}
lineView.measure.heights=null
if(lineView.rest){for(var i=0;i<lineView.rest.length;i++)
{lineView.measure.caches[i]={}}}}}
function clearLineMeasurementCache(cm){cm.display.externalMeasure=null
removeChildren(cm.display.lineMeasure)
for(var i=0;i<cm.display.view.length;i++)
{clearLineMeasurementCacheFor(cm.display.view[i])}}
function clearCaches(cm){clearLineMeasurementCache(cm)
cm.display.cachedCharWidth=cm.display.cachedTextHeight=cm.display.cachedPaddingH=null
if(!cm.options.lineWrapping){cm.display.maxLineChanged=true}
cm.display.lineNumChars=null}
function pageScrollX(){return window.pageXOffset||(document.documentElement||document.body).scrollLeft}
function pageScrollY(){return window.pageYOffset||(document.documentElement||document.body).scrollTop}
function intoCoordSystem(cm,lineObj,rect,context,includeWidgets){if(!includeWidgets&&lineObj.widgets){for(var i=0;i<lineObj.widgets.length;++i){if(lineObj.widgets[i].above){var size=widgetHeight(lineObj.widgets[i])
rect.top+=size;rect.bottom+=size}}}
if(context=="line"){return rect}
if(!context){context="local"}
var yOff=heightAtLine(lineObj)
if(context=="local"){yOff+=paddingTop(cm.display)}
else{yOff-=cm.display.viewOffset}
if(context=="page"||context=="window"){var lOff=cm.display.lineSpace.getBoundingClientRect()
yOff+=lOff.top+(context=="window"?0:pageScrollY())
var xOff=lOff.left+(context=="window"?0:pageScrollX())
rect.left+=xOff;rect.right+=xOff}
rect.top+=yOff;rect.bottom+=yOff
return rect}
function fromCoordSystem(cm,coords,context){if(context=="div"){return coords}
var left=coords.left,top=coords.top
if(context=="page"){left-=pageScrollX()
top-=pageScrollY()}else if(context=="local"||!context){var localBox=cm.display.sizer.getBoundingClientRect()
left+=localBox.left
top+=localBox.top}
var lineSpaceBox=cm.display.lineSpace.getBoundingClientRect()
return{left:left-lineSpaceBox.left,top:top-lineSpaceBox.top}}
function charCoords(cm,pos,context,lineObj,bias){if(!lineObj){lineObj=getLine(cm.doc,pos.line)}
return intoCoordSystem(cm,lineObj,measureChar(cm,lineObj,pos.ch,bias),context)}
function cursorCoords(cm,pos,context,lineObj,preparedMeasure,varHeight){lineObj=lineObj||getLine(cm.doc,pos.line)
if(!preparedMeasure){preparedMeasure=prepareMeasureForLine(cm,lineObj)}
function get(ch,right){var m=measureCharPrepared(cm,preparedMeasure,ch,right?"right":"left",varHeight)
if(right){m.left=m.right;}else{m.right=m.left}
return intoCoordSystem(cm,lineObj,m,context)}
var order=getOrder(lineObj,cm.doc.direction),ch=pos.ch,sticky=pos.sticky
if(ch>=lineObj.text.length){ch=lineObj.text.length
sticky="before"}else if(ch<=0){ch=0
sticky="after"}
if(!order){return get(sticky=="before"?ch-1:ch,sticky=="before")}
function getBidi(ch,partPos,invert){var part=order[partPos],right=(part.level%2)!=0
return get(invert?ch-1:ch,right!=invert)}
var partPos=getBidiPartAt(order,ch,sticky)
var other=bidiOther
var val=getBidi(ch,partPos,sticky=="before")
if(other!=null){val.other=getBidi(ch,other,sticky!="before")}
return val}
function estimateCoords(cm,pos){var left=0
pos=clipPos(cm.doc,pos)
if(!cm.options.lineWrapping){left=charWidth(cm.display)*pos.ch}
var lineObj=getLine(cm.doc,pos.line)
var top=heightAtLine(lineObj)+paddingTop(cm.display)
return{left:left,right:left,top:top,bottom:top+lineObj.height}}
function PosWithInfo(line,ch,sticky,outside,xRel){var pos=Pos(line,ch,sticky)
pos.xRel=xRel
if(outside){pos.outside=true}
return pos}
function coordsChar(cm,x,y){var doc=cm.doc
y+=cm.display.viewOffset
if(y<0){return PosWithInfo(doc.first,0,null,true,-1)}
var lineN=lineAtHeight(doc,y),last=doc.first+doc.size-1
if(lineN>last)
{return PosWithInfo(doc.first+doc.size-1,getLine(doc,last).text.length,null,true,1)}
if(x<0){x=0}
var lineObj=getLine(doc,lineN)
for(;;){var found=coordsCharInner(cm,lineObj,lineN,x,y)
var merged=collapsedSpanAtEnd(lineObj)
var mergedPos=merged&&merged.find(0,true)
if(merged&&(found.ch>mergedPos.from.ch||found.ch==mergedPos.from.ch&&found.xRel>0))
{lineN=lineNo(lineObj=mergedPos.to.line)}
else
{return found}}}
function wrappedLineExtent(cm,lineObj,preparedMeasure,y){var measure=function(ch){return intoCoordSystem(cm,lineObj,measureCharPrepared(cm,preparedMeasure,ch),"line");}
var end=lineObj.text.length
var begin=findFirst(function(ch){return measure(ch-1).bottom<=y;},end,0)
end=findFirst(function(ch){return measure(ch).top>y;},begin,end)
return{begin:begin,end:end}}
function wrappedLineExtentChar(cm,lineObj,preparedMeasure,target){var targetTop=intoCoordSystem(cm,lineObj,measureCharPrepared(cm,preparedMeasure,target),"line").top
return wrappedLineExtent(cm,lineObj,preparedMeasure,targetTop)}
function coordsCharInner(cm,lineObj,lineNo,x,y){y-=heightAtLine(lineObj)
var begin=0,end=lineObj.text.length
var preparedMeasure=prepareMeasureForLine(cm,lineObj)
var pos
var order=getOrder(lineObj,cm.doc.direction)
if(order){if(cm.options.lineWrapping){;var assign;((assign=wrappedLineExtent(cm,lineObj,preparedMeasure,y),begin=assign.begin,end=assign.end))}
pos=new Pos(lineNo,begin)
var beginLeft=cursorCoords(cm,pos,"line",lineObj,preparedMeasure).left
var dir=beginLeft<x?1:-1
var prevDiff,diff=beginLeft-x,prevPos
do{prevDiff=diff
prevPos=pos
pos=moveVisually(cm,lineObj,pos,dir)
if(pos==null||pos.ch<begin||end<=(pos.sticky=="before"?pos.ch-1:pos.ch)){pos=prevPos
break}
diff=cursorCoords(cm,pos,"line",lineObj,preparedMeasure).left-x}while((dir<0)!=(diff<0)&&(Math.abs(diff)<=Math.abs(prevDiff)))
if(Math.abs(diff)>Math.abs(prevDiff)){if((diff<0)==(prevDiff<0)){throw new Error("Broke out of infinite loop in coordsCharInner")}
pos=prevPos}}else{var ch=findFirst(function(ch){var box=intoCoordSystem(cm,lineObj,measureCharPrepared(cm,preparedMeasure,ch),"line")
if(box.top>y){end=Math.min(ch,end)
return true}
else if(box.bottom<=y){return false}
else if(box.left>x){return true}
else if(box.right<x){return false}
else{return(x-box.left<box.right-x)}},begin,end)
ch=skipExtendingChars(lineObj.text,ch,1)
pos=new Pos(lineNo,ch,ch==end?"before":"after")}
var coords=cursorCoords(cm,pos,"line",lineObj,preparedMeasure)
if(y<coords.top||coords.bottom<y){pos.outside=true}
pos.xRel=x<coords.left?-1:(x>coords.right?1:0)
return pos}
var measureText
function textHeight(display){if(display.cachedTextHeight!=null){return display.cachedTextHeight}
if(measureText==null){measureText=elt("pre")
for(var i=0;i<49;++i){measureText.appendChild(document.createTextNode("x"))
measureText.appendChild(elt("br"))}
measureText.appendChild(document.createTextNode("x"))}
removeChildrenAndAdd(display.measure,measureText)
var height=measureText.offsetHeight/50
if(height>3){display.cachedTextHeight=height}
removeChildren(display.measure)
return height||1}
function charWidth(display){if(display.cachedCharWidth!=null){return display.cachedCharWidth}
var anchor=elt("span","xxxxxxxxxx")
var pre=elt("pre",[anchor])
removeChildrenAndAdd(display.measure,pre)
var rect=anchor.getBoundingClientRect(),width=(rect.right-rect.left)/10
if(width>2){display.cachedCharWidth=width}
return width||10}
function getDimensions(cm){var d=cm.display,left={},width={}
var gutterLeft=d.gutters.clientLeft
for(var n=d.gutters.firstChild,i=0;n;n=n.nextSibling,++i){left[cm.options.gutters[i]]=n.offsetLeft+n.clientLeft+gutterLeft
width[cm.options.gutters[i]]=n.clientWidth}
return{fixedPos:compensateForHScroll(d),gutterTotalWidth:d.gutters.offsetWidth,gutterLeft:left,gutterWidth:width,wrapperWidth:d.wrapper.clientWidth}}
function compensateForHScroll(display){return display.scroller.getBoundingClientRect().left-display.sizer.getBoundingClientRect().left}
function estimateHeight(cm){var th=textHeight(cm.display),wrapping=cm.options.lineWrapping
var perLine=wrapping&&Math.max(5,cm.display.scroller.clientWidth/charWidth(cm.display)-3)
return function(line){if(lineIsHidden(cm.doc,line)){return 0}
var widgetsHeight=0
if(line.widgets){for(var i=0;i<line.widgets.length;i++){if(line.widgets[i].height){widgetsHeight+=line.widgets[i].height}}}
if(wrapping)
{return widgetsHeight+(Math.ceil(line.text.length/perLine)||1)*th}
else
{return widgetsHeight+th}}}
function estimateLineHeights(cm){var doc=cm.doc,est=estimateHeight(cm)
doc.iter(function(line){var estHeight=est(line)
if(estHeight!=line.height){updateLineHeight(line,estHeight)}})}
function posFromMouse(cm,e,liberal,forRect){var display=cm.display
if(!liberal&&e_target(e).getAttribute("cm-not-content")=="true"){return null}
var x,y,space=display.lineSpace.getBoundingClientRect()
try{x=e.clientX-space.left;y=e.clientY-space.top}
catch(e){return null}
var coords=coordsChar(cm,x,y),line
if(forRect&&coords.xRel==1&&(line=getLine(cm.doc,coords.line).text).length==coords.ch){var colDiff=countColumn(line,line.length,cm.options.tabSize)-line.length
coords=Pos(coords.line,Math.max(0,Math.round((x-paddingH(cm.display).left)/charWidth(cm.display))-colDiff))}
return coords}
function findViewIndex(cm,n){if(n>=cm.display.viewTo){return null}
n-=cm.display.viewFrom
if(n<0){return null}
var view=cm.display.view
for(var i=0;i<view.length;i++){n-=view[i].size
if(n<0){return i}}}
function updateSelection(cm){cm.display.input.showSelection(cm.display.input.prepareSelection())}
function prepareSelection(cm,primary){var doc=cm.doc,result={}
var curFragment=result.cursors=document.createDocumentFragment()
var selFragment=result.selection=document.createDocumentFragment()
for(var i=0;i<doc.sel.ranges.length;i++){if(primary===false&&i==doc.sel.primIndex){continue}
var range=doc.sel.ranges[i]
if(range.from().line>=cm.display.viewTo||range.to().line<cm.display.viewFrom){continue}
var collapsed=range.empty()
if(collapsed||cm.options.showCursorWhenSelecting)
{drawSelectionCursor(cm,range.head,curFragment)}
if(!collapsed)
{drawSelectionRange(cm,range,selFragment)}}
return result}
function drawSelectionCursor(cm,head,output){var pos=cursorCoords(cm,head,"div",null,null,!cm.options.singleCursorHeightPerLine)
var cursor=output.appendChild(elt("div","\u00a0","CodeMirror-cursor"))
cursor.style.left=pos.left+"px"
cursor.style.top=pos.top+"px"
cursor.style.height=Math.max(0,pos.bottom-pos.top)*cm.options.cursorHeight+"px"
if(pos.other){var otherCursor=output.appendChild(elt("div","\u00a0","CodeMirror-cursor CodeMirror-secondarycursor"))
otherCursor.style.display=""
otherCursor.style.left=pos.other.left+"px"
otherCursor.style.top=pos.other.top+"px"
otherCursor.style.height=(pos.other.bottom-pos.other.top)*.85+"px"}}
function drawSelectionRange(cm,range,output){var display=cm.display,doc=cm.doc
var fragment=document.createDocumentFragment()
var padding=paddingH(cm.display),leftSide=padding.left
var rightSide=Math.max(display.sizerWidth,displayWidth(cm)-display.sizer.offsetLeft)-padding.right
function add(left,top,width,bottom){if(top<0){top=0}
top=Math.round(top)
bottom=Math.round(bottom)
fragment.appendChild(elt("div",null,"CodeMirror-selected",("position: absolute; left: "+left+"px;\n                             top: "+top+"px; width: "+(width==null?rightSide-left:width)+"px;\n                             height: "+(bottom-top)+"px")))}
function drawForLine(line,fromArg,toArg){var lineObj=getLine(doc,line)
var lineLen=lineObj.text.length
var start,end
function coords(ch,bias){return charCoords(cm,Pos(line,ch),"div",lineObj,bias)}
iterateBidiSections(getOrder(lineObj,doc.direction),fromArg||0,toArg==null?lineLen:toArg,function(from,to,dir){var leftPos=coords(from,"left"),rightPos,left,right
if(from==to){rightPos=leftPos
left=right=leftPos.left}else{rightPos=coords(to-1,"right")
if(dir=="rtl"){var tmp=leftPos;leftPos=rightPos;rightPos=tmp}
left=leftPos.left
right=rightPos.right}
if(fromArg==null&&from==0){left=leftSide}
if(rightPos.top-leftPos.top>3){add(left,leftPos.top,null,leftPos.bottom)
left=leftSide
if(leftPos.bottom<rightPos.top){add(left,leftPos.bottom,null,rightPos.top)}}
if(toArg==null&&to==lineLen){right=rightSide}
if(!start||leftPos.top<start.top||leftPos.top==start.top&&leftPos.left<start.left)
{start=leftPos}
if(!end||rightPos.bottom>end.bottom||rightPos.bottom==end.bottom&&rightPos.right>end.right)
{end=rightPos}
if(left<leftSide+1){left=leftSide}
add(left,rightPos.top,right-left,rightPos.bottom)})
return{start:start,end:end}}
var sFrom=range.from(),sTo=range.to()
if(sFrom.line==sTo.line){drawForLine(sFrom.line,sFrom.ch,sTo.ch)}else{var fromLine=getLine(doc,sFrom.line),toLine=getLine(doc,sTo.line)
var singleVLine=visualLine(fromLine)==visualLine(toLine)
var leftEnd=drawForLine(sFrom.line,sFrom.ch,singleVLine?fromLine.text.length+1:null).end
var rightStart=drawForLine(sTo.line,singleVLine?0:null,sTo.ch).start
if(singleVLine){if(leftEnd.top<rightStart.top-2){add(leftEnd.right,leftEnd.top,null,leftEnd.bottom)
add(leftSide,rightStart.top,rightStart.left,rightStart.bottom)}else{add(leftEnd.right,leftEnd.top,rightStart.left-leftEnd.right,leftEnd.bottom)}}
if(leftEnd.bottom<rightStart.top)
{add(leftSide,leftEnd.bottom,null,rightStart.top)}}
output.appendChild(fragment)}
function restartBlink(cm){if(!cm.state.focused){return}
var display=cm.display
clearInterval(display.blinker)
var on=true
display.cursorDiv.style.visibility=""
if(cm.options.cursorBlinkRate>0)
{display.blinker=setInterval(function(){return display.cursorDiv.style.visibility=(on=!on)?"":"hidden";},cm.options.cursorBlinkRate)}
else if(cm.options.cursorBlinkRate<0)
{display.cursorDiv.style.visibility="hidden"}}
function ensureFocus(cm){if(!cm.state.focused){cm.display.input.focus();onFocus(cm)}}
function delayBlurEvent(cm){cm.state.delayingBlurEvent=true
setTimeout(function(){if(cm.state.delayingBlurEvent){cm.state.delayingBlurEvent=false
onBlur(cm)}},100)}
function onFocus(cm,e){if(cm.state.delayingBlurEvent){cm.state.delayingBlurEvent=false}
if(cm.options.readOnly=="nocursor"){return}
if(!cm.state.focused){signal(cm,"focus",cm,e)
cm.state.focused=true
addClass(cm.display.wrapper,"CodeMirror-focused")
if(!cm.curOp&&cm.display.selForContextMenu!=cm.doc.sel){cm.display.input.reset()
if(webkit){setTimeout(function(){return cm.display.input.reset(true);},20)}}
cm.display.input.receivedFocus()}
restartBlink(cm)}
function onBlur(cm,e){if(cm.state.delayingBlurEvent){return}
if(cm.state.focused){signal(cm,"blur",cm,e)
cm.state.focused=false
rmClass(cm.display.wrapper,"CodeMirror-focused")}
clearInterval(cm.display.blinker)
setTimeout(function(){if(!cm.state.focused){cm.display.shift=false}},150)}
function alignHorizontally(cm){var display=cm.display,view=display.view
if(!display.alignWidgets&&(!display.gutters.firstChild||!cm.options.fixedGutter)){return}
var comp=compensateForHScroll(display)-display.scroller.scrollLeft+cm.doc.scrollLeft
var gutterW=display.gutters.offsetWidth,left=comp+"px"
for(var i=0;i<view.length;i++){if(!view[i].hidden){if(cm.options.fixedGutter){if(view[i].gutter)
{view[i].gutter.style.left=left}
if(view[i].gutterBackground)
{view[i].gutterBackground.style.left=left}}
var align=view[i].alignable
if(align){for(var j=0;j<align.length;j++)
{align[j].style.left=left}}}}
if(cm.options.fixedGutter)
{display.gutters.style.left=(comp+gutterW)+"px"}}
function maybeUpdateLineNumberWidth(cm){if(!cm.options.lineNumbers){return false}
var doc=cm.doc,last=lineNumberFor(cm.options,doc.first+doc.size-1),display=cm.display
if(last.length!=display.lineNumChars){var test=display.measure.appendChild(elt("div",[elt("div",last)],"CodeMirror-linenumber CodeMirror-gutter-elt"))
var innerW=test.firstChild.offsetWidth,padding=test.offsetWidth-innerW
display.lineGutter.style.width=""
display.lineNumInnerWidth=Math.max(innerW,display.lineGutter.offsetWidth-padding)+1
display.lineNumWidth=display.lineNumInnerWidth+padding
display.lineNumChars=display.lineNumInnerWidth?last.length:-1
display.lineGutter.style.width=display.lineNumWidth+"px"
updateGutterSpace(cm)
return true}
return false}
function updateHeightsInViewport(cm){var display=cm.display
var prevBottom=display.lineDiv.offsetTop
for(var i=0;i<display.view.length;i++){var cur=display.view[i],height=(void 0)
if(cur.hidden){continue}
if(ie&&ie_version<8){var bot=cur.node.offsetTop+cur.node.offsetHeight
height=bot-prevBottom
prevBottom=bot}else{var box=cur.node.getBoundingClientRect()
height=box.bottom-box.top}
var diff=cur.line.height-height
if(height<2){height=textHeight(display)}
if(diff>.001||diff<-.001){updateLineHeight(cur.line,height)
updateWidgetHeight(cur.line)
if(cur.rest){for(var j=0;j<cur.rest.length;j++)
{updateWidgetHeight(cur.rest[j])}}}}}
function updateWidgetHeight(line){if(line.widgets){for(var i=0;i<line.widgets.length;++i)
{line.widgets[i].height=line.widgets[i].node.parentNode.offsetHeight}}}
function visibleLines(display,doc,viewport){var top=viewport&&viewport.top!=null?Math.max(0,viewport.top):display.scroller.scrollTop
top=Math.floor(top-paddingTop(display))
var bottom=viewport&&viewport.bottom!=null?viewport.bottom:top+display.wrapper.clientHeight
var from=lineAtHeight(doc,top),to=lineAtHeight(doc,bottom)
if(viewport&&viewport.ensure){var ensureFrom=viewport.ensure.from.line,ensureTo=viewport.ensure.to.line
if(ensureFrom<from){from=ensureFrom
to=lineAtHeight(doc,heightAtLine(getLine(doc,ensureFrom))+display.wrapper.clientHeight)}else if(Math.min(ensureTo,doc.lastLine())>=to){from=lineAtHeight(doc,heightAtLine(getLine(doc,ensureTo))-display.wrapper.clientHeight)
to=ensureTo}}
return{from:from,to:Math.max(to,from+1)}}
function setScrollTop(cm,val){if(Math.abs(cm.doc.scrollTop-val)<2){return}
cm.doc.scrollTop=val
if(!gecko){updateDisplaySimple(cm,{top:val})}
if(cm.display.scroller.scrollTop!=val){cm.display.scroller.scrollTop=val}
cm.display.scrollbars.setScrollTop(val)
if(gecko){updateDisplaySimple(cm)}
startWorker(cm,100)}
function setScrollLeft(cm,val,isScroller){if(isScroller?val==cm.doc.scrollLeft:Math.abs(cm.doc.scrollLeft-val)<2){return}
val=Math.min(val,cm.display.scroller.scrollWidth-cm.display.scroller.clientWidth)
cm.doc.scrollLeft=val
alignHorizontally(cm)
if(cm.display.scroller.scrollLeft!=val){cm.display.scroller.scrollLeft=val}
cm.display.scrollbars.setScrollLeft(val)}
var wheelSamples=0;var wheelPixelsPerUnit=null;if(ie){wheelPixelsPerUnit=-.53}
else if(gecko){wheelPixelsPerUnit=15}
else if(chrome){wheelPixelsPerUnit=-.7}
else if(safari){wheelPixelsPerUnit=-1/3}
function wheelEventDelta(e){var dx=e.wheelDeltaX,dy=e.wheelDeltaY
if(dx==null&&e.detail&&e.axis==e.HORIZONTAL_AXIS){dx=e.detail}
if(dy==null&&e.detail&&e.axis==e.VERTICAL_AXIS){dy=e.detail}
else if(dy==null){dy=e.wheelDelta}
return{x:dx,y:dy}}
function wheelEventPixels(e){var delta=wheelEventDelta(e)
delta.x*=wheelPixelsPerUnit
delta.y*=wheelPixelsPerUnit
return delta}
function onScrollWheel(cm,e){var delta=wheelEventDelta(e),dx=delta.x,dy=delta.y
var display=cm.display,scroll=display.scroller
var canScrollX=scroll.scrollWidth>scroll.clientWidth
var canScrollY=scroll.scrollHeight>scroll.clientHeight
if(!(dx&&canScrollX||dy&&canScrollY)){return}
if(dy&&mac&&webkit){outer:for(var cur=e.target,view=display.view;cur!=scroll;cur=cur.parentNode){for(var i=0;i<view.length;i++){if(view[i].node==cur){cm.display.currentWheelTarget=cur
break outer}}}}
if(dx&&!gecko&&!presto&&wheelPixelsPerUnit!=null){if(dy&&canScrollY)
{setScrollTop(cm,Math.max(0,Math.min(scroll.scrollTop+dy*wheelPixelsPerUnit,scroll.scrollHeight-scroll.clientHeight)))}
setScrollLeft(cm,Math.max(0,Math.min(scroll.scrollLeft+dx*wheelPixelsPerUnit,scroll.scrollWidth-scroll.clientWidth)))
if(!dy||(dy&&canScrollY))
{e_preventDefault(e)}
display.wheelStartX=null
return}
if(dy&&wheelPixelsPerUnit!=null){var pixels=dy*wheelPixelsPerUnit
var top=cm.doc.scrollTop,bot=top+display.wrapper.clientHeight
if(pixels<0){top=Math.max(0,top+pixels-50)}
else{bot=Math.min(cm.doc.height,bot+pixels+50)}
updateDisplaySimple(cm,{top:top,bottom:bot})}
if(wheelSamples<20){if(display.wheelStartX==null){display.wheelStartX=scroll.scrollLeft;display.wheelStartY=scroll.scrollTop
display.wheelDX=dx;display.wheelDY=dy
setTimeout(function(){if(display.wheelStartX==null){return}
var movedX=scroll.scrollLeft-display.wheelStartX
var movedY=scroll.scrollTop-display.wheelStartY
var sample=(movedY&&display.wheelDY&&movedY/display.wheelDY)||(movedX&&display.wheelDX&&movedX/display.wheelDX)
display.wheelStartX=display.wheelStartY=null
if(!sample){return}
wheelPixelsPerUnit=(wheelPixelsPerUnit*wheelSamples+sample)/(wheelSamples+1)
++wheelSamples},200)}else{display.wheelDX+=dx;display.wheelDY+=dy}}}
function measureForScrollbars(cm){var d=cm.display,gutterW=d.gutters.offsetWidth
var docH=Math.round(cm.doc.height+paddingVert(cm.display))
return{clientHeight:d.scroller.clientHeight,viewHeight:d.wrapper.clientHeight,scrollWidth:d.scroller.scrollWidth,clientWidth:d.scroller.clientWidth,viewWidth:d.wrapper.clientWidth,barLeft:cm.options.fixedGutter?gutterW:0,docHeight:docH,scrollHeight:docH+scrollGap(cm)+d.barHeight,nativeBarWidth:d.nativeBarWidth,gutterWidth:gutterW}}
var NativeScrollbars=function NativeScrollbars(place,scroll,cm){this.cm=cm
var vert=this.vert=elt("div",[elt("div",null,null,"min-width: 1px")],"CodeMirror-vscrollbar")
var horiz=this.horiz=elt("div",[elt("div",null,null,"height: 100%; min-height: 1px")],"CodeMirror-hscrollbar")
place(vert);place(horiz)
on(vert,"scroll",function(){if(vert.clientHeight){scroll(vert.scrollTop,"vertical")}})
on(horiz,"scroll",function(){if(horiz.clientWidth){scroll(horiz.scrollLeft,"horizontal")}})
this.checkedZeroWidth=false
if(ie&&ie_version<8){this.horiz.style.minHeight=this.vert.style.minWidth="18px"}};NativeScrollbars.prototype.update=function update(measure){var needsH=measure.scrollWidth>measure.clientWidth+1
var needsV=measure.scrollHeight>measure.clientHeight+1
var sWidth=measure.nativeBarWidth
if(needsV){this.vert.style.display="block"
this.vert.style.bottom=needsH?sWidth+"px":"0"
var totalHeight=measure.viewHeight-(needsH?sWidth:0)
this.vert.firstChild.style.height=Math.max(0,measure.scrollHeight-measure.clientHeight+totalHeight)+"px"}else{this.vert.style.display=""
this.vert.firstChild.style.height="0"}
if(needsH){this.horiz.style.display="block"
this.horiz.style.right=needsV?sWidth+"px":"0"
this.horiz.style.left=measure.barLeft+"px"
var totalWidth=measure.viewWidth-measure.barLeft-(needsV?sWidth:0)
this.horiz.firstChild.style.width=Math.max(0,measure.scrollWidth-measure.clientWidth+totalWidth)+"px"}else{this.horiz.style.display=""
this.horiz.firstChild.style.width="0"}
if(!this.checkedZeroWidth&&measure.clientHeight>0){if(sWidth==0){this.zeroWidthHack()}
this.checkedZeroWidth=true}
return{right:needsV?sWidth:0,bottom:needsH?sWidth:0}};NativeScrollbars.prototype.setScrollLeft=function setScrollLeft$1(pos){if(this.horiz.scrollLeft!=pos){this.horiz.scrollLeft=pos}
if(this.disableHoriz){this.enableZeroWidthBar(this.horiz,this.disableHoriz)}};NativeScrollbars.prototype.setScrollTop=function setScrollTop$1(pos){if(this.vert.scrollTop!=pos){this.vert.scrollTop=pos}
if(this.disableVert){this.enableZeroWidthBar(this.vert,this.disableVert)}};NativeScrollbars.prototype.zeroWidthHack=function zeroWidthHack(){var w=mac&&!mac_geMountainLion?"12px":"18px"
this.horiz.style.height=this.vert.style.width=w
this.horiz.style.pointerEvents=this.vert.style.pointerEvents="none"
this.disableHoriz=new Delayed
this.disableVert=new Delayed};NativeScrollbars.prototype.enableZeroWidthBar=function enableZeroWidthBar(bar,delay){bar.style.pointerEvents="auto"
function maybeDisable(){var box=bar.getBoundingClientRect()
var elt=document.elementFromPoint(box.left+1,box.bottom-1)
if(elt!=bar){bar.style.pointerEvents="none"}
else{delay.set(1000,maybeDisable)}}
delay.set(1000,maybeDisable)};NativeScrollbars.prototype.clear=function clear(){var parent=this.horiz.parentNode
parent.removeChild(this.horiz)
parent.removeChild(this.vert)};var NullScrollbars=function NullScrollbars(){};NullScrollbars.prototype.update=function update(){return{bottom:0,right:0}};NullScrollbars.prototype.setScrollLeft=function setScrollLeft$2(){};NullScrollbars.prototype.setScrollTop=function setScrollTop$2(){};NullScrollbars.prototype.clear=function clear(){};function updateScrollbars(cm,measure){if(!measure){measure=measureForScrollbars(cm)}
var startWidth=cm.display.barWidth,startHeight=cm.display.barHeight
updateScrollbarsInner(cm,measure)
for(var i=0;i<4&&startWidth!=cm.display.barWidth||startHeight!=cm.display.barHeight;i++){if(startWidth!=cm.display.barWidth&&cm.options.lineWrapping)
{updateHeightsInViewport(cm)}
updateScrollbarsInner(cm,measureForScrollbars(cm))
startWidth=cm.display.barWidth;startHeight=cm.display.barHeight}}
function updateScrollbarsInner(cm,measure){var d=cm.display
var sizes=d.scrollbars.update(measure)
d.sizer.style.paddingRight=(d.barWidth=sizes.right)+"px"
d.sizer.style.paddingBottom=(d.barHeight=sizes.bottom)+"px"
d.heightForcer.style.borderBottom=sizes.bottom+"px solid transparent"
if(sizes.right&&sizes.bottom){d.scrollbarFiller.style.display="block"
d.scrollbarFiller.style.height=sizes.bottom+"px"
d.scrollbarFiller.style.width=sizes.right+"px"}else{d.scrollbarFiller.style.display=""}
if(sizes.bottom&&cm.options.coverGutterNextToScrollbar&&cm.options.fixedGutter){d.gutterFiller.style.display="block"
d.gutterFiller.style.height=sizes.bottom+"px"
d.gutterFiller.style.width=measure.gutterWidth+"px"}else{d.gutterFiller.style.display=""}}
var scrollbarModel={"native":NativeScrollbars,"null":NullScrollbars}
function initScrollbars(cm){if(cm.display.scrollbars){cm.display.scrollbars.clear()
if(cm.display.scrollbars.addClass)
{rmClass(cm.display.wrapper,cm.display.scrollbars.addClass)}}
cm.display.scrollbars=new scrollbarModel[cm.options.scrollbarStyle](function(node){cm.display.wrapper.insertBefore(node,cm.display.scrollbarFiller)
on(node,"mousedown",function(){if(cm.state.focused){setTimeout(function(){return cm.display.input.focus();},0)}})
node.setAttribute("cm-not-content","true")},function(pos,axis){if(axis=="horizontal"){setScrollLeft(cm,pos)}
else{setScrollTop(cm,pos)}},cm)
if(cm.display.scrollbars.addClass)
{addClass(cm.display.wrapper,cm.display.scrollbars.addClass)}}
function maybeScrollWindow(cm,rect){if(signalDOMEvent(cm,"scrollCursorIntoView")){return}
var display=cm.display,box=display.sizer.getBoundingClientRect(),doScroll=null
if(rect.top+box.top<0){doScroll=true}
else if(rect.bottom+box.top>(window.innerHeight||document.documentElement.clientHeight)){doScroll=false}
if(doScroll!=null&&!phantom){var scrollNode=elt("div","\u200b",null,("position: absolute;\n                         top: "+(rect.top-display.viewOffset-paddingTop(cm.display))+"px;\n                         height: "+(rect.bottom-rect.top+scrollGap(cm)+display.barHeight)+"px;\n                         left: "+(rect.left)+"px; width: "+(Math.max(2,rect.right-rect.left))+"px;"))
cm.display.lineSpace.appendChild(scrollNode)
scrollNode.scrollIntoView(doScroll)
cm.display.lineSpace.removeChild(scrollNode)}}
function scrollPosIntoView(cm,pos,end,margin){if(margin==null){margin=0}
var rect
for(var limit=0;limit<5;limit++){var changed=false
var coords=cursorCoords(cm,pos)
var endCoords=!end||end==pos?coords:cursorCoords(cm,end)
rect={left:Math.min(coords.left,endCoords.left),top:Math.min(coords.top,endCoords.top)-margin,right:Math.max(coords.left,endCoords.left),bottom:Math.max(coords.bottom,endCoords.bottom)+margin}
var scrollPos=calculateScrollPos(cm,rect)
var startTop=cm.doc.scrollTop,startLeft=cm.doc.scrollLeft
if(scrollPos.scrollTop!=null){setScrollTop(cm,scrollPos.scrollTop)
if(Math.abs(cm.doc.scrollTop-startTop)>1){changed=true}}
if(scrollPos.scrollLeft!=null){setScrollLeft(cm,scrollPos.scrollLeft)
if(Math.abs(cm.doc.scrollLeft-startLeft)>1){changed=true}}
if(!changed){break}}
return rect}
function scrollIntoView(cm,rect){var scrollPos=calculateScrollPos(cm,rect)
if(scrollPos.scrollTop!=null){setScrollTop(cm,scrollPos.scrollTop)}
if(scrollPos.scrollLeft!=null){setScrollLeft(cm,scrollPos.scrollLeft)}}
function calculateScrollPos(cm,rect){var display=cm.display,snapMargin=textHeight(cm.display)
if(rect.top<0){rect.top=0}
var screentop=cm.curOp&&cm.curOp.scrollTop!=null?cm.curOp.scrollTop:display.scroller.scrollTop
var screen=displayHeight(cm),result={}
if(rect.bottom-rect.top>screen){rect.bottom=rect.top+screen}
var docBottom=cm.doc.height+paddingVert(display)
var atTop=rect.top<snapMargin,atBottom=rect.bottom>docBottom-snapMargin
if(rect.top<screentop){result.scrollTop=atTop?0:rect.top}else if(rect.bottom>screentop+screen){var newTop=Math.min(rect.top,(atBottom?docBottom:rect.bottom)-screen)
if(newTop!=screentop){result.scrollTop=newTop}}
var screenleft=cm.curOp&&cm.curOp.scrollLeft!=null?cm.curOp.scrollLeft:display.scroller.scrollLeft
var screenw=displayWidth(cm)-(cm.options.fixedGutter?display.gutters.offsetWidth:0)
var tooWide=rect.right-rect.left>screenw
if(tooWide){rect.right=rect.left+screenw}
if(rect.left<10)
{result.scrollLeft=0}
else if(rect.left<screenleft)
{result.scrollLeft=Math.max(0,rect.left-(tooWide?0:10))}
else if(rect.right>screenw+screenleft-3)
{result.scrollLeft=rect.right+(tooWide?0:10)-screenw}
return result}
function addToScrollPos(cm,left,top){if(left!=null||top!=null){resolveScrollToPos(cm)}
if(left!=null)
{cm.curOp.scrollLeft=(cm.curOp.scrollLeft==null?cm.doc.scrollLeft:cm.curOp.scrollLeft)+left}
if(top!=null)
{cm.curOp.scrollTop=(cm.curOp.scrollTop==null?cm.doc.scrollTop:cm.curOp.scrollTop)+top}}
function ensureCursorVisible(cm){resolveScrollToPos(cm)
var cur=cm.getCursor(),from=cur,to=cur
if(!cm.options.lineWrapping){from=cur.ch?Pos(cur.line,cur.ch-1):cur
to=Pos(cur.line,cur.ch+1)}
cm.curOp.scrollToPos={from:from,to:to,margin:cm.options.cursorScrollMargin}}
function resolveScrollToPos(cm){var range=cm.curOp.scrollToPos
if(range){cm.curOp.scrollToPos=null
var from=estimateCoords(cm,range.from),to=estimateCoords(cm,range.to)
var sPos=calculateScrollPos(cm,{left:Math.min(from.left,to.left),top:Math.min(from.top,to.top)-range.margin,right:Math.max(from.right,to.right),bottom:Math.max(from.bottom,to.bottom)+range.margin})
cm.scrollTo(sPos.scrollLeft,sPos.scrollTop)}}
var nextOpId=0
function startOperation(cm){cm.curOp={cm:cm,viewChanged:false,startHeight:cm.doc.height,forceUpdate:false,updateInput:null,typing:false,changeObjs:null,cursorActivityHandlers:null,cursorActivityCalled:0,selectionChanged:false,updateMaxLine:false,scrollLeft:null,scrollTop:null,scrollToPos:null,focus:false,id:++nextOpId}
pushOperation(cm.curOp)}
function endOperation(cm){var op=cm.curOp
finishOperation(op,function(group){for(var i=0;i<group.ops.length;i++)
{group.ops[i].cm.curOp=null}
endOperations(group)})}
function endOperations(group){var ops=group.ops
for(var i=0;i<ops.length;i++)
{endOperation_R1(ops[i])}
for(var i$1=0;i$1<ops.length;i$1++)
{endOperation_W1(ops[i$1])}
for(var i$2=0;i$2<ops.length;i$2++)
{endOperation_R2(ops[i$2])}
for(var i$3=0;i$3<ops.length;i$3++)
{endOperation_W2(ops[i$3])}
for(var i$4=0;i$4<ops.length;i$4++)
{endOperation_finish(ops[i$4])}}
function endOperation_R1(op){var cm=op.cm,display=cm.display
maybeClipScrollbars(cm)
if(op.updateMaxLine){findMaxLine(cm)}
op.mustUpdate=op.viewChanged||op.forceUpdate||op.scrollTop!=null||op.scrollToPos&&(op.scrollToPos.from.line<display.viewFrom||op.scrollToPos.to.line>=display.viewTo)||display.maxLineChanged&&cm.options.lineWrapping
op.update=op.mustUpdate&&new DisplayUpdate(cm,op.mustUpdate&&{top:op.scrollTop,ensure:op.scrollToPos},op.forceUpdate)}
function endOperation_W1(op){op.updatedDisplay=op.mustUpdate&&updateDisplayIfNeeded(op.cm,op.update)}
function endOperation_R2(op){var cm=op.cm,display=cm.display
if(op.updatedDisplay){updateHeightsInViewport(cm)}
op.barMeasure=measureForScrollbars(cm)
if(display.maxLineChanged&&!cm.options.lineWrapping){op.adjustWidthTo=measureChar(cm,display.maxLine,display.maxLine.text.length).left+3
cm.display.sizerWidth=op.adjustWidthTo
op.barMeasure.scrollWidth=Math.max(display.scroller.clientWidth,display.sizer.offsetLeft+op.adjustWidthTo+scrollGap(cm)+cm.display.barWidth)
op.maxScrollLeft=Math.max(0,display.sizer.offsetLeft+op.adjustWidthTo-displayWidth(cm))}
if(op.updatedDisplay||op.selectionChanged)
{op.preparedSelection=display.input.prepareSelection(op.focus)}}
function endOperation_W2(op){var cm=op.cm
if(op.adjustWidthTo!=null){cm.display.sizer.style.minWidth=op.adjustWidthTo+"px"
if(op.maxScrollLeft<cm.doc.scrollLeft)
{setScrollLeft(cm,Math.min(cm.display.scroller.scrollLeft,op.maxScrollLeft),true)}
cm.display.maxLineChanged=false}
var takeFocus=op.focus&&op.focus==activeElt()&&(!document.hasFocus||document.hasFocus())
if(op.preparedSelection)
{cm.display.input.showSelection(op.preparedSelection,takeFocus)}
if(op.updatedDisplay||op.startHeight!=cm.doc.height)
{updateScrollbars(cm,op.barMeasure)}
if(op.updatedDisplay)
{setDocumentHeight(cm,op.barMeasure)}
if(op.selectionChanged){restartBlink(cm)}
if(cm.state.focused&&op.updateInput)
{cm.display.input.reset(op.typing)}
if(takeFocus){ensureFocus(op.cm)}}
function endOperation_finish(op){var cm=op.cm,display=cm.display,doc=cm.doc
if(op.updatedDisplay){postUpdateDisplay(cm,op.update)}
if(display.wheelStartX!=null&&(op.scrollTop!=null||op.scrollLeft!=null||op.scrollToPos))
{display.wheelStartX=display.wheelStartY=null}
if(op.scrollTop!=null&&(display.scroller.scrollTop!=op.scrollTop||op.forceScroll)){doc.scrollTop=Math.max(0,Math.min(display.scroller.scrollHeight-display.scroller.clientHeight,op.scrollTop))
display.scrollbars.setScrollTop(doc.scrollTop)
display.scroller.scrollTop=doc.scrollTop}
if(op.scrollLeft!=null&&(display.scroller.scrollLeft!=op.scrollLeft||op.forceScroll)){doc.scrollLeft=Math.max(0,Math.min(display.scroller.scrollWidth-display.scroller.clientWidth,op.scrollLeft))
display.scrollbars.setScrollLeft(doc.scrollLeft)
display.scroller.scrollLeft=doc.scrollLeft
alignHorizontally(cm)}
if(op.scrollToPos){var rect=scrollPosIntoView(cm,clipPos(doc,op.scrollToPos.from),clipPos(doc,op.scrollToPos.to),op.scrollToPos.margin)
maybeScrollWindow(cm,rect)}
var hidden=op.maybeHiddenMarkers,unhidden=op.maybeUnhiddenMarkers
if(hidden){for(var i=0;i<hidden.length;++i)
{if(!hidden[i].lines.length){signal(hidden[i],"hide")}}}
if(unhidden){for(var i$1=0;i$1<unhidden.length;++i$1)
{if(unhidden[i$1].lines.length){signal(unhidden[i$1],"unhide")}}}
if(display.wrapper.offsetHeight)
{doc.scrollTop=cm.display.scroller.scrollTop}
if(op.changeObjs)
{signal(cm,"changes",cm,op.changeObjs)}
if(op.update)
{op.update.finish()}}
function runInOp(cm,f){if(cm.curOp){return f()}
startOperation(cm)
try{return f()}
finally{endOperation(cm)}}
function operation(cm,f){return function(){if(cm.curOp){return f.apply(cm,arguments)}
startOperation(cm)
try{return f.apply(cm,arguments)}
finally{endOperation(cm)}}}
function methodOp(f){return function(){if(this.curOp){return f.apply(this,arguments)}
startOperation(this)
try{return f.apply(this,arguments)}
finally{endOperation(this)}}}
function docMethodOp(f){return function(){var cm=this.cm
if(!cm||cm.curOp){return f.apply(this,arguments)}
startOperation(cm)
try{return f.apply(this,arguments)}
finally{endOperation(cm)}}}
function regChange(cm,from,to,lendiff){if(from==null){from=cm.doc.first}
if(to==null){to=cm.doc.first+cm.doc.size}
if(!lendiff){lendiff=0}
var display=cm.display
if(lendiff&&to<display.viewTo&&(display.updateLineNumbers==null||display.updateLineNumbers>from))
{display.updateLineNumbers=from}
cm.curOp.viewChanged=true
if(from>=display.viewTo){if(sawCollapsedSpans&&visualLineNo(cm.doc,from)<display.viewTo)
{resetView(cm)}}else if(to<=display.viewFrom){if(sawCollapsedSpans&&visualLineEndNo(cm.doc,to+lendiff)>display.viewFrom){resetView(cm)}else{display.viewFrom+=lendiff
display.viewTo+=lendiff}}else if(from<=display.viewFrom&&to>=display.viewTo){resetView(cm)}else if(from<=display.viewFrom){var cut=viewCuttingPoint(cm,to,to+lendiff,1)
if(cut){display.view=display.view.slice(cut.index)
display.viewFrom=cut.lineN
display.viewTo+=lendiff}else{resetView(cm)}}else if(to>=display.viewTo){var cut$1=viewCuttingPoint(cm,from,from,-1)
if(cut$1){display.view=display.view.slice(0,cut$1.index)
display.viewTo=cut$1.lineN}else{resetView(cm)}}else{var cutTop=viewCuttingPoint(cm,from,from,-1)
var cutBot=viewCuttingPoint(cm,to,to+lendiff,1)
if(cutTop&&cutBot){display.view=display.view.slice(0,cutTop.index).concat(buildViewArray(cm,cutTop.lineN,cutBot.lineN)).concat(display.view.slice(cutBot.index))
display.viewTo+=lendiff}else{resetView(cm)}}
var ext=display.externalMeasured
if(ext){if(to<ext.lineN)
{ext.lineN+=lendiff}
else if(from<ext.lineN+ext.size)
{display.externalMeasured=null}}}
function regLineChange(cm,line,type){cm.curOp.viewChanged=true
var display=cm.display,ext=cm.display.externalMeasured
if(ext&&line>=ext.lineN&&line<ext.lineN+ext.size)
{display.externalMeasured=null}
if(line<display.viewFrom||line>=display.viewTo){return}
var lineView=display.view[findViewIndex(cm,line)]
if(lineView.node==null){return}
var arr=lineView.changes||(lineView.changes=[])
if(indexOf(arr,type)==-1){arr.push(type)}}
function resetView(cm){cm.display.viewFrom=cm.display.viewTo=cm.doc.first
cm.display.view=[]
cm.display.viewOffset=0}
function viewCuttingPoint(cm,oldN,newN,dir){var index=findViewIndex(cm,oldN),diff,view=cm.display.view
if(!sawCollapsedSpans||newN==cm.doc.first+cm.doc.size)
{return{index:index,lineN:newN}}
var n=cm.display.viewFrom
for(var i=0;i<index;i++)
{n+=view[i].size}
if(n!=oldN){if(dir>0){if(index==view.length-1){return null}
diff=(n+view[index].size)-oldN
index++}else{diff=n-oldN}
oldN+=diff;newN+=diff}
while(visualLineNo(cm.doc,newN)!=newN){if(index==(dir<0?0:view.length-1)){return null}
newN+=dir*view[index-(dir<0?1:0)].size
index+=dir}
return{index:index,lineN:newN}}
function adjustView(cm,from,to){var display=cm.display,view=display.view
if(view.length==0||from>=display.viewTo||to<=display.viewFrom){display.view=buildViewArray(cm,from,to)
display.viewFrom=from}else{if(display.viewFrom>from)
{display.view=buildViewArray(cm,from,display.viewFrom).concat(display.view)}
else if(display.viewFrom<from)
{display.view=display.view.slice(findViewIndex(cm,from))}
display.viewFrom=from
if(display.viewTo<to)
{display.view=display.view.concat(buildViewArray(cm,display.viewTo,to))}
else if(display.viewTo>to)
{display.view=display.view.slice(0,findViewIndex(cm,to))}}
display.viewTo=to}
function countDirtyView(cm){var view=cm.display.view,dirty=0
for(var i=0;i<view.length;i++){var lineView=view[i]
if(!lineView.hidden&&(!lineView.node||lineView.changes)){++dirty}}
return dirty}
function startWorker(cm,time){if(cm.doc.mode.startState&&cm.doc.frontier<cm.display.viewTo)
{cm.state.highlight.set(time,bind(highlightWorker,cm))}}
function highlightWorker(cm){var doc=cm.doc
if(doc.frontier<doc.first){doc.frontier=doc.first}
if(doc.frontier>=cm.display.viewTo){return}
var end=+new Date+cm.options.workTime
var state=copyState(doc.mode,getStateBefore(cm,doc.frontier))
var changedLines=[]
doc.iter(doc.frontier,Math.min(doc.first+doc.size,cm.display.viewTo+500),function(line){if(doc.frontier>=cm.display.viewFrom){var oldStyles=line.styles,tooLong=line.text.length>cm.options.maxHighlightLength
var highlighted=highlightLine(cm,line,tooLong?copyState(doc.mode,state):state,true)
line.styles=highlighted.styles
var oldCls=line.styleClasses,newCls=highlighted.classes
if(newCls){line.styleClasses=newCls}
else if(oldCls){line.styleClasses=null}
var ischange=!oldStyles||oldStyles.length!=line.styles.length||oldCls!=newCls&&(!oldCls||!newCls||oldCls.bgClass!=newCls.bgClass||oldCls.textClass!=newCls.textClass)
for(var i=0;!ischange&&i<oldStyles.length;++i){ischange=oldStyles[i]!=line.styles[i]}
if(ischange){changedLines.push(doc.frontier)}
line.stateAfter=tooLong?state:copyState(doc.mode,state)}else{if(line.text.length<=cm.options.maxHighlightLength)
{processLine(cm,line.text,state)}
line.stateAfter=doc.frontier%5==0?copyState(doc.mode,state):null}
++doc.frontier
if(+new Date>end){startWorker(cm,cm.options.workDelay)
return true}})
if(changedLines.length){runInOp(cm,function(){for(var i=0;i<changedLines.length;i++)
{regLineChange(cm,changedLines[i],"text")}})}}
var DisplayUpdate=function DisplayUpdate(cm,viewport,force){var display=cm.display
this.viewport=viewport
this.visible=visibleLines(display,cm.doc,viewport)
this.editorIsHidden=!display.wrapper.offsetWidth
this.wrapperHeight=display.wrapper.clientHeight
this.wrapperWidth=display.wrapper.clientWidth
this.oldDisplayWidth=displayWidth(cm)
this.force=force
this.dims=getDimensions(cm)
this.events=[]};DisplayUpdate.prototype.signal=function signal$1(emitter,type){if(hasHandler(emitter,type))
{this.events.push(arguments)}};DisplayUpdate.prototype.finish=function finish(){var this$1=this;for(var i=0;i<this.events.length;i++)
{signal.apply(null,this$1.events[i])}};function maybeClipScrollbars(cm){var display=cm.display
if(!display.scrollbarsClipped&&display.scroller.offsetWidth){display.nativeBarWidth=display.scroller.offsetWidth-display.scroller.clientWidth
display.heightForcer.style.height=scrollGap(cm)+"px"
display.sizer.style.marginBottom=-display.nativeBarWidth+"px"
display.sizer.style.borderRightWidth=scrollGap(cm)+"px"
display.scrollbarsClipped=true}}
function updateDisplayIfNeeded(cm,update){var display=cm.display,doc=cm.doc
if(update.editorIsHidden){resetView(cm)
return false}
if(!update.force&&update.visible.from>=display.viewFrom&&update.visible.to<=display.viewTo&&(display.updateLineNumbers==null||display.updateLineNumbers>=display.viewTo)&&display.renderedView==display.view&&countDirtyView(cm)==0)
{return false}
if(maybeUpdateLineNumberWidth(cm)){resetView(cm)
update.dims=getDimensions(cm)}
var end=doc.first+doc.size
var from=Math.max(update.visible.from-cm.options.viewportMargin,doc.first)
var to=Math.min(end,update.visible.to+cm.options.viewportMargin)
if(display.viewFrom<from&&from-display.viewFrom<20){from=Math.max(doc.first,display.viewFrom)}
if(display.viewTo>to&&display.viewTo-to<20){to=Math.min(end,display.viewTo)}
if(sawCollapsedSpans){from=visualLineNo(cm.doc,from)
to=visualLineEndNo(cm.doc,to)}
var different=from!=display.viewFrom||to!=display.viewTo||display.lastWrapHeight!=update.wrapperHeight||display.lastWrapWidth!=update.wrapperWidth
adjustView(cm,from,to)
display.viewOffset=heightAtLine(getLine(cm.doc,display.viewFrom))
cm.display.mover.style.top=display.viewOffset+"px"
var toUpdate=countDirtyView(cm)
if(!different&&toUpdate==0&&!update.force&&display.renderedView==display.view&&(display.updateLineNumbers==null||display.updateLineNumbers>=display.viewTo))
{return false}
var focused=activeElt()
if(toUpdate>4){display.lineDiv.style.display="none"}
patchDisplay(cm,display.updateLineNumbers,update.dims)
if(toUpdate>4){display.lineDiv.style.display=""}
display.renderedView=display.view
if(focused&&activeElt()!=focused&&focused.offsetHeight){focused.focus()}
removeChildren(display.cursorDiv)
removeChildren(display.selectionDiv)
display.gutters.style.height=display.sizer.style.minHeight=0
if(different){display.lastWrapHeight=update.wrapperHeight
display.lastWrapWidth=update.wrapperWidth
startWorker(cm,400)}
display.updateLineNumbers=null
return true}
function postUpdateDisplay(cm,update){var viewport=update.viewport
for(var first=true;;first=false){if(!first||!cm.options.lineWrapping||update.oldDisplayWidth==displayWidth(cm)){if(viewport&&viewport.top!=null)
{viewport={top:Math.min(cm.doc.height+paddingVert(cm.display)-displayHeight(cm),viewport.top)}}
update.visible=visibleLines(cm.display,cm.doc,viewport)
if(update.visible.from>=cm.display.viewFrom&&update.visible.to<=cm.display.viewTo)
{break}}
if(!updateDisplayIfNeeded(cm,update)){break}
updateHeightsInViewport(cm)
var barMeasure=measureForScrollbars(cm)
updateSelection(cm)
updateScrollbars(cm,barMeasure)
setDocumentHeight(cm,barMeasure)}
update.signal(cm,"update",cm)
if(cm.display.viewFrom!=cm.display.reportedViewFrom||cm.display.viewTo!=cm.display.reportedViewTo){update.signal(cm,"viewportChange",cm,cm.display.viewFrom,cm.display.viewTo)
cm.display.reportedViewFrom=cm.display.viewFrom;cm.display.reportedViewTo=cm.display.viewTo}}
function updateDisplaySimple(cm,viewport){var update=new DisplayUpdate(cm,viewport)
if(updateDisplayIfNeeded(cm,update)){updateHeightsInViewport(cm)
postUpdateDisplay(cm,update)
var barMeasure=measureForScrollbars(cm)
updateSelection(cm)
updateScrollbars(cm,barMeasure)
setDocumentHeight(cm,barMeasure)
update.finish()}}
function patchDisplay(cm,updateNumbersFrom,dims){var display=cm.display,lineNumbers=cm.options.lineNumbers
var container=display.lineDiv,cur=container.firstChild
function rm(node){var next=node.nextSibling
if(webkit&&mac&&cm.display.currentWheelTarget==node)
{node.style.display="none"}
else
{node.parentNode.removeChild(node)}
return next}
var view=display.view,lineN=display.viewFrom
for(var i=0;i<view.length;i++){var lineView=view[i]
if(lineView.hidden){}else if(!lineView.node||lineView.node.parentNode!=container){var node=buildLineElement(cm,lineView,lineN,dims)
container.insertBefore(node,cur)}else{while(cur!=lineView.node){cur=rm(cur)}
var updateNumber=lineNumbers&&updateNumbersFrom!=null&&updateNumbersFrom<=lineN&&lineView.lineNumber
if(lineView.changes){if(indexOf(lineView.changes,"gutter")>-1){updateNumber=false}
updateLineForChanges(cm,lineView,lineN,dims)}
if(updateNumber){removeChildren(lineView.lineNumber)
lineView.lineNumber.appendChild(document.createTextNode(lineNumberFor(cm.options,lineN)))}
cur=lineView.node.nextSibling}
lineN+=lineView.size}
while(cur){cur=rm(cur)}}
function updateGutterSpace(cm){var width=cm.display.gutters.offsetWidth
cm.display.sizer.style.marginLeft=width+"px"}
function setDocumentHeight(cm,measure){cm.display.sizer.style.minHeight=measure.docHeight+"px"
cm.display.heightForcer.style.top=measure.docHeight+"px"
cm.display.gutters.style.height=(measure.docHeight+cm.display.barHeight+scrollGap(cm))+"px"}
function updateGutters(cm){var gutters=cm.display.gutters,specs=cm.options.gutters
removeChildren(gutters)
var i=0
for(;i<specs.length;++i){var gutterClass=specs[i]
var gElt=gutters.appendChild(elt("div",null,"CodeMirror-gutter "+gutterClass))
if(gutterClass=="CodeMirror-linenumbers"){cm.display.lineGutter=gElt
gElt.style.width=(cm.display.lineNumWidth||1)+"px"}}
gutters.style.display=i?"":"none"
updateGutterSpace(cm)}
function setGuttersForLineNumbers(options){var found=indexOf(options.gutters,"CodeMirror-linenumbers")
if(found==-1&&options.lineNumbers){options.gutters=options.gutters.concat(["CodeMirror-linenumbers"])}else if(found>-1&&!options.lineNumbers){options.gutters=options.gutters.slice(0)
options.gutters.splice(found,1)}}
var Selection=function Selection(ranges,primIndex){this.ranges=ranges
this.primIndex=primIndex};Selection.prototype.primary=function primary(){return this.ranges[this.primIndex]};Selection.prototype.equals=function equals(other){var this$1=this;if(other==this){return true}
if(other.primIndex!=this.primIndex||other.ranges.length!=this.ranges.length){return false}
for(var i=0;i<this.ranges.length;i++){var here=this$1.ranges[i],there=other.ranges[i]
if(!equalCursorPos(here.anchor,there.anchor)||!equalCursorPos(here.head,there.head)){return false}}
return true};Selection.prototype.deepCopy=function deepCopy(){var this$1=this;var out=[]
for(var i=0;i<this.ranges.length;i++)
{out[i]=new Range(copyPos(this$1.ranges[i].anchor),copyPos(this$1.ranges[i].head))}
return new Selection(out,this.primIndex)};Selection.prototype.somethingSelected=function somethingSelected(){var this$1=this;for(var i=0;i<this.ranges.length;i++)
{if(!this$1.ranges[i].empty()){return true}}
return false};Selection.prototype.contains=function contains(pos,end){var this$1=this;if(!end){end=pos}
for(var i=0;i<this.ranges.length;i++){var range=this$1.ranges[i]
if(cmp(end,range.from())>=0&&cmp(pos,range.to())<=0)
{return i}}
return-1};var Range=function Range(anchor,head){this.anchor=anchor;this.head=head};Range.prototype.from=function from(){return minPos(this.anchor,this.head)};Range.prototype.to=function to(){return maxPos(this.anchor,this.head)};Range.prototype.empty=function empty(){return this.head.line==this.anchor.line&&this.head.ch==this.anchor.ch};function normalizeSelection(ranges,primIndex){var prim=ranges[primIndex]
ranges.sort(function(a,b){return cmp(a.from(),b.from());})
primIndex=indexOf(ranges,prim)
for(var i=1;i<ranges.length;i++){var cur=ranges[i],prev=ranges[i-1]
if(cmp(prev.to(),cur.from())>=0){var from=minPos(prev.from(),cur.from()),to=maxPos(prev.to(),cur.to())
var inv=prev.empty()?cur.from()==cur.head:prev.from()==prev.head
if(i<=primIndex){--primIndex}
ranges.splice(--i,2,new Range(inv?to:from,inv?from:to))}}
return new Selection(ranges,primIndex)}
function simpleSelection(anchor,head){return new Selection([new Range(anchor,head||anchor)],0)}
function changeEnd(change){if(!change.text){return change.to}
return Pos(change.from.line+change.text.length-1,lst(change.text).length+(change.text.length==1?change.from.ch:0))}
function adjustForChange(pos,change){if(cmp(pos,change.from)<0){return pos}
if(cmp(pos,change.to)<=0){return changeEnd(change)}
var line=pos.line+change.text.length-(change.to.line-change.from.line)-1,ch=pos.ch
if(pos.line==change.to.line){ch+=changeEnd(change).ch-change.to.ch}
return Pos(line,ch)}
function computeSelAfterChange(doc,change){var out=[]
for(var i=0;i<doc.sel.ranges.length;i++){var range=doc.sel.ranges[i]
out.push(new Range(adjustForChange(range.anchor,change),adjustForChange(range.head,change)))}
return normalizeSelection(out,doc.sel.primIndex)}
function offsetPos(pos,old,nw){if(pos.line==old.line)
{return Pos(nw.line,pos.ch-old.ch+nw.ch)}
else
{return Pos(nw.line+(pos.line-old.line),pos.ch)}}
function computeReplacedSel(doc,changes,hint){var out=[]
var oldPrev=Pos(doc.first,0),newPrev=oldPrev
for(var i=0;i<changes.length;i++){var change=changes[i]
var from=offsetPos(change.from,oldPrev,newPrev)
var to=offsetPos(changeEnd(change),oldPrev,newPrev)
oldPrev=change.to
newPrev=to
if(hint=="around"){var range=doc.sel.ranges[i],inv=cmp(range.head,range.anchor)<0
out[i]=new Range(inv?to:from,inv?from:to)}else{out[i]=new Range(from,from)}}
return new Selection(out,doc.sel.primIndex)}
function loadMode(cm){cm.doc.mode=getMode(cm.options,cm.doc.modeOption)
resetModeState(cm)}
function resetModeState(cm){cm.doc.iter(function(line){if(line.stateAfter){line.stateAfter=null}
if(line.styles){line.styles=null}})
cm.doc.frontier=cm.doc.first
startWorker(cm,100)
cm.state.modeGen++
if(cm.curOp){regChange(cm)}}
function isWholeLineUpdate(doc,change){return change.from.ch==0&&change.to.ch==0&&lst(change.text)==""&&(!doc.cm||doc.cm.options.wholeLineUpdateBefore)}
function updateDoc(doc,change,markedSpans,estimateHeight){function spansFor(n){return markedSpans?markedSpans[n]:null}
function update(line,text,spans){updateLine(line,text,spans,estimateHeight)
signalLater(line,"change",line,change)}
function linesFor(start,end){var result=[]
for(var i=start;i<end;++i)
{result.push(new Line(text[i],spansFor(i),estimateHeight))}
return result}
var from=change.from,to=change.to,text=change.text
var firstLine=getLine(doc,from.line),lastLine=getLine(doc,to.line)
var lastText=lst(text),lastSpans=spansFor(text.length-1),nlines=to.line-from.line
if(change.full){doc.insert(0,linesFor(0,text.length))
doc.remove(text.length,doc.size-text.length)}else if(isWholeLineUpdate(doc,change)){var added=linesFor(0,text.length-1)
update(lastLine,lastLine.text,lastSpans)
if(nlines){doc.remove(from.line,nlines)}
if(added.length){doc.insert(from.line,added)}}else if(firstLine==lastLine){if(text.length==1){update(firstLine,firstLine.text.slice(0,from.ch)+lastText+firstLine.text.slice(to.ch),lastSpans)}else{var added$1=linesFor(1,text.length-1)
added$1.push(new Line(lastText+firstLine.text.slice(to.ch),lastSpans,estimateHeight))
update(firstLine,firstLine.text.slice(0,from.ch)+text[0],spansFor(0))
doc.insert(from.line+1,added$1)}}else if(text.length==1){update(firstLine,firstLine.text.slice(0,from.ch)+text[0]+lastLine.text.slice(to.ch),spansFor(0))
doc.remove(from.line+1,nlines)}else{update(firstLine,firstLine.text.slice(0,from.ch)+text[0],spansFor(0))
update(lastLine,lastText+lastLine.text.slice(to.ch),lastSpans)
var added$2=linesFor(1,text.length-1)
if(nlines>1){doc.remove(from.line+1,nlines-1)}
doc.insert(from.line+1,added$2)}
signalLater(doc,"change",doc,change)}
function linkedDocs(doc,f,sharedHistOnly){function propagate(doc,skip,sharedHist){if(doc.linked){for(var i=0;i<doc.linked.length;++i){var rel=doc.linked[i]
if(rel.doc==skip){continue}
var shared=sharedHist&&rel.sharedHist
if(sharedHistOnly&&!shared){continue}
f(rel.doc,shared)
propagate(rel.doc,doc,shared)}}}
propagate(doc,null,true)}
function attachDoc(cm,doc){if(doc.cm){throw new Error("This document is already in use.")}
cm.doc=doc
doc.cm=cm
estimateLineHeights(cm)
loadMode(cm)
setDirectionClass(cm)
if(!cm.options.lineWrapping){findMaxLine(cm)}
cm.options.mode=doc.modeOption
regChange(cm)}
function setDirectionClass(cm){;(cm.doc.direction=="rtl"?addClass:rmClass)(cm.display.lineDiv,"CodeMirror-rtl")}
function directionChanged(cm){runInOp(cm,function(){setDirectionClass(cm)
regChange(cm)})}
function History(startGen){this.done=[];this.undone=[]
this.undoDepth=Infinity
this.lastModTime=this.lastSelTime=0
this.lastOp=this.lastSelOp=null
this.lastOrigin=this.lastSelOrigin=null
this.generation=this.maxGeneration=startGen||1}
function historyChangeFromChange(doc,change){var histChange={from:copyPos(change.from),to:changeEnd(change),text:getBetween(doc,change.from,change.to)}
attachLocalSpans(doc,histChange,change.from.line,change.to.line+1)
linkedDocs(doc,function(doc){return attachLocalSpans(doc,histChange,change.from.line,change.to.line+1);},true)
return histChange}
function clearSelectionEvents(array){while(array.length){var last=lst(array)
if(last.ranges){array.pop()}
else{break}}}
function lastChangeEvent(hist,force){if(force){clearSelectionEvents(hist.done)
return lst(hist.done)}else if(hist.done.length&&!lst(hist.done).ranges){return lst(hist.done)}else if(hist.done.length>1&&!hist.done[hist.done.length-2].ranges){hist.done.pop()
return lst(hist.done)}}
function addChangeToHistory(doc,change,selAfter,opId){var hist=doc.history
hist.undone.length=0
var time=+new Date,cur
var last
if((hist.lastOp==opId||hist.lastOrigin==change.origin&&change.origin&&((change.origin.charAt(0)=="+"&&doc.cm&&hist.lastModTime>time-doc.cm.options.historyEventDelay)||change.origin.charAt(0)=="*"))&&(cur=lastChangeEvent(hist,hist.lastOp==opId))){last=lst(cur.changes)
if(cmp(change.from,change.to)==0&&cmp(change.from,last.to)==0){last.to=changeEnd(change)}else{cur.changes.push(historyChangeFromChange(doc,change))}}else{var before=lst(hist.done)
if(!before||!before.ranges)
{pushSelectionToHistory(doc.sel,hist.done)}
cur={changes:[historyChangeFromChange(doc,change)],generation:hist.generation}
hist.done.push(cur)
while(hist.done.length>hist.undoDepth){hist.done.shift()
if(!hist.done[0].ranges){hist.done.shift()}}}
hist.done.push(selAfter)
hist.generation=++hist.maxGeneration
hist.lastModTime=hist.lastSelTime=time
hist.lastOp=hist.lastSelOp=opId
hist.lastOrigin=hist.lastSelOrigin=change.origin
if(!last){signal(doc,"historyAdded")}}
function selectionEventCanBeMerged(doc,origin,prev,sel){var ch=origin.charAt(0)
return ch=="*"||ch=="+"&&prev.ranges.length==sel.ranges.length&&prev.somethingSelected()==sel.somethingSelected()&&new Date-doc.history.lastSelTime<=(doc.cm?doc.cm.options.historyEventDelay:500)}
function addSelectionToHistory(doc,sel,opId,options){var hist=doc.history,origin=options&&options.origin
if(opId==hist.lastSelOp||(origin&&hist.lastSelOrigin==origin&&(hist.lastModTime==hist.lastSelTime&&hist.lastOrigin==origin||selectionEventCanBeMerged(doc,origin,lst(hist.done),sel))))
{hist.done[hist.done.length-1]=sel}
else
{pushSelectionToHistory(sel,hist.done)}
hist.lastSelTime=+new Date
hist.lastSelOrigin=origin
hist.lastSelOp=opId
if(options&&options.clearRedo!==false)
{clearSelectionEvents(hist.undone)}}
function pushSelectionToHistory(sel,dest){var top=lst(dest)
if(!(top&&top.ranges&&top.equals(sel)))
{dest.push(sel)}}
function attachLocalSpans(doc,change,from,to){var existing=change["spans_"+doc.id],n=0
doc.iter(Math.max(doc.first,from),Math.min(doc.first+doc.size,to),function(line){if(line.markedSpans)
{(existing||(existing=change["spans_"+doc.id]={}))[n]=line.markedSpans}
++n})}
function removeClearedSpans(spans){if(!spans){return null}
var out
for(var i=0;i<spans.length;++i){if(spans[i].marker.explicitlyCleared){if(!out){out=spans.slice(0,i)}}
else if(out){out.push(spans[i])}}
return!out?spans:out.length?out:null}
function getOldSpans(doc,change){var found=change["spans_"+doc.id]
if(!found){return null}
var nw=[]
for(var i=0;i<change.text.length;++i)
{nw.push(removeClearedSpans(found[i]))}
return nw}
function mergeOldSpans(doc,change){var old=getOldSpans(doc,change)
var stretched=stretchSpansOverChange(doc,change)
if(!old){return stretched}
if(!stretched){return old}
for(var i=0;i<old.length;++i){var oldCur=old[i],stretchCur=stretched[i]
if(oldCur&&stretchCur){spans:for(var j=0;j<stretchCur.length;++j){var span=stretchCur[j]
for(var k=0;k<oldCur.length;++k)
{if(oldCur[k].marker==span.marker){continue spans}}
oldCur.push(span)}}else if(stretchCur){old[i]=stretchCur}}
return old}
function copyHistoryArray(events,newGroup,instantiateSel){var copy=[]
for(var i=0;i<events.length;++i){var event=events[i]
if(event.ranges){copy.push(instantiateSel?Selection.prototype.deepCopy.call(event):event)
continue}
var changes=event.changes,newChanges=[]
copy.push({changes:newChanges})
for(var j=0;j<changes.length;++j){var change=changes[j],m=(void 0)
newChanges.push({from:change.from,to:change.to,text:change.text})
if(newGroup){for(var prop in change){if(m=prop.match(/^spans_(\d+)$/)){if(indexOf(newGroup,Number(m[1]))>-1){lst(newChanges)[prop]=change[prop]
delete change[prop]}}}}}}
return copy}
function extendRange(doc,range,head,other){if(doc.cm&&doc.cm.display.shift||doc.extend){var anchor=range.anchor
if(other){var posBefore=cmp(head,anchor)<0
if(posBefore!=(cmp(other,anchor)<0)){anchor=head
head=other}else if(posBefore!=(cmp(head,other)<0)){head=other}}
return new Range(anchor,head)}else{return new Range(other||head,head)}}
function extendSelection(doc,head,other,options){setSelection(doc,new Selection([extendRange(doc,doc.sel.primary(),head,other)],0),options)}
function extendSelections(doc,heads,options){var out=[]
for(var i=0;i<doc.sel.ranges.length;i++)
{out[i]=extendRange(doc,doc.sel.ranges[i],heads[i],null)}
var newSel=normalizeSelection(out,doc.sel.primIndex)
setSelection(doc,newSel,options)}
function replaceOneSelection(doc,i,range,options){var ranges=doc.sel.ranges.slice(0)
ranges[i]=range
setSelection(doc,normalizeSelection(ranges,doc.sel.primIndex),options)}
function setSimpleSelection(doc,anchor,head,options){setSelection(doc,simpleSelection(anchor,head),options)}
function filterSelectionChange(doc,sel,options){var obj={ranges:sel.ranges,update:function(ranges){var this$1=this;this.ranges=[]
for(var i=0;i<ranges.length;i++)
{this$1.ranges[i]=new Range(clipPos(doc,ranges[i].anchor),clipPos(doc,ranges[i].head))}},origin:options&&options.origin}
signal(doc,"beforeSelectionChange",doc,obj)
if(doc.cm){signal(doc.cm,"beforeSelectionChange",doc.cm,obj)}
if(obj.ranges!=sel.ranges){return normalizeSelection(obj.ranges,obj.ranges.length-1)}
else{return sel}}
function setSelectionReplaceHistory(doc,sel,options){var done=doc.history.done,last=lst(done)
if(last&&last.ranges){done[done.length-1]=sel
setSelectionNoUndo(doc,sel,options)}else{setSelection(doc,sel,options)}}
function setSelection(doc,sel,options){setSelectionNoUndo(doc,sel,options)
addSelectionToHistory(doc,doc.sel,doc.cm?doc.cm.curOp.id:NaN,options)}
function setSelectionNoUndo(doc,sel,options){if(hasHandler(doc,"beforeSelectionChange")||doc.cm&&hasHandler(doc.cm,"beforeSelectionChange"))
{sel=filterSelectionChange(doc,sel,options)}
var bias=options&&options.bias||(cmp(sel.primary().head,doc.sel.primary().head)<0?-1:1)
setSelectionInner(doc,skipAtomicInSelection(doc,sel,bias,true))
if(!(options&&options.scroll===false)&&doc.cm)
{ensureCursorVisible(doc.cm)}}
function setSelectionInner(doc,sel){if(sel.equals(doc.sel)){return}
doc.sel=sel
if(doc.cm){doc.cm.curOp.updateInput=doc.cm.curOp.selectionChanged=true
signalCursorActivity(doc.cm)}
signalLater(doc,"cursorActivity",doc)}
function reCheckSelection(doc){setSelectionInner(doc,skipAtomicInSelection(doc,doc.sel,null,false),sel_dontScroll)}
function skipAtomicInSelection(doc,sel,bias,mayClear){var out
for(var i=0;i<sel.ranges.length;i++){var range=sel.ranges[i]
var old=sel.ranges.length==doc.sel.ranges.length&&doc.sel.ranges[i]
var newAnchor=skipAtomic(doc,range.anchor,old&&old.anchor,bias,mayClear)
var newHead=skipAtomic(doc,range.head,old&&old.head,bias,mayClear)
if(out||newAnchor!=range.anchor||newHead!=range.head){if(!out){out=sel.ranges.slice(0,i)}
out[i]=new Range(newAnchor,newHead)}}
return out?normalizeSelection(out,sel.primIndex):sel}
function skipAtomicInner(doc,pos,oldPos,dir,mayClear){var line=getLine(doc,pos.line)
if(line.markedSpans){for(var i=0;i<line.markedSpans.length;++i){var sp=line.markedSpans[i],m=sp.marker
if((sp.from==null||(m.inclusiveLeft?sp.from<=pos.ch:sp.from<pos.ch))&&(sp.to==null||(m.inclusiveRight?sp.to>=pos.ch:sp.to>pos.ch))){if(mayClear){signal(m,"beforeCursorEnter")
if(m.explicitlyCleared){if(!line.markedSpans){break}
else{--i;continue}}}
if(!m.atomic){continue}
if(oldPos){var near=m.find(dir<0?1:-1),diff=(void 0)
if(dir<0?m.inclusiveRight:m.inclusiveLeft)
{near=movePos(doc,near,-dir,near&&near.line==pos.line?line:null)}
if(near&&near.line==pos.line&&(diff=cmp(near,oldPos))&&(dir<0?diff<0:diff>0))
{return skipAtomicInner(doc,near,pos,dir,mayClear)}}
var far=m.find(dir<0?-1:1)
if(dir<0?m.inclusiveLeft:m.inclusiveRight)
{far=movePos(doc,far,dir,far.line==pos.line?line:null)}
return far?skipAtomicInner(doc,far,pos,dir,mayClear):null}}}
return pos}
function skipAtomic(doc,pos,oldPos,bias,mayClear){var dir=bias||1
var found=skipAtomicInner(doc,pos,oldPos,dir,mayClear)||(!mayClear&&skipAtomicInner(doc,pos,oldPos,dir,true))||skipAtomicInner(doc,pos,oldPos,-dir,mayClear)||(!mayClear&&skipAtomicInner(doc,pos,oldPos,-dir,true))
if(!found){doc.cantEdit=true
return Pos(doc.first,0)}
return found}
function movePos(doc,pos,dir,line){if(dir<0&&pos.ch==0){if(pos.line>doc.first){return clipPos(doc,Pos(pos.line-1))}
else{return null}}else if(dir>0&&pos.ch==(line||getLine(doc,pos.line)).text.length){if(pos.line<doc.first+doc.size-1){return Pos(pos.line+1,0)}
else{return null}}else{return new Pos(pos.line,pos.ch+dir)}}
function selectAll(cm){cm.setSelection(Pos(cm.firstLine(),0),Pos(cm.lastLine()),sel_dontScroll)}
function filterChange(doc,change,update){var obj={canceled:false,from:change.from,to:change.to,text:change.text,origin:change.origin,cancel:function(){return obj.canceled=true;}}
if(update){obj.update=function(from,to,text,origin){if(from){obj.from=clipPos(doc,from)}
if(to){obj.to=clipPos(doc,to)}
if(text){obj.text=text}
if(origin!==undefined){obj.origin=origin}}}
signal(doc,"beforeChange",doc,obj)
if(doc.cm){signal(doc.cm,"beforeChange",doc.cm,obj)}
if(obj.canceled){return null}
return{from:obj.from,to:obj.to,text:obj.text,origin:obj.origin}}
function makeChange(doc,change,ignoreReadOnly){if(doc.cm){if(!doc.cm.curOp){return operation(doc.cm,makeChange)(doc,change,ignoreReadOnly)}
if(doc.cm.state.suppressEdits){return}}
if(hasHandler(doc,"beforeChange")||doc.cm&&hasHandler(doc.cm,"beforeChange")){change=filterChange(doc,change,true)
if(!change){return}}
var split=sawReadOnlySpans&&!ignoreReadOnly&&removeReadOnlyRanges(doc,change.from,change.to)
if(split){for(var i=split.length-1;i>=0;--i)
{makeChangeInner(doc,{from:split[i].from,to:split[i].to,text:i?[""]:change.text})}}else{makeChangeInner(doc,change)}}
function makeChangeInner(doc,change){if(change.text.length==1&&change.text[0]==""&&cmp(change.from,change.to)==0){return}
var selAfter=computeSelAfterChange(doc,change)
addChangeToHistory(doc,change,selAfter,doc.cm?doc.cm.curOp.id:NaN)
makeChangeSingleDoc(doc,change,selAfter,stretchSpansOverChange(doc,change))
var rebased=[]
linkedDocs(doc,function(doc,sharedHist){if(!sharedHist&&indexOf(rebased,doc.history)==-1){rebaseHist(doc.history,change)
rebased.push(doc.history)}
makeChangeSingleDoc(doc,change,null,stretchSpansOverChange(doc,change))})}
function makeChangeFromHistory(doc,type,allowSelectionOnly){if(doc.cm&&doc.cm.state.suppressEdits&&!allowSelectionOnly){return}
var hist=doc.history,event,selAfter=doc.sel
var source=type=="undo"?hist.done:hist.undone,dest=type=="undo"?hist.undone:hist.done
var i=0
for(;i<source.length;i++){event=source[i]
if(allowSelectionOnly?event.ranges&&!event.equals(doc.sel):!event.ranges)
{break}}
if(i==source.length){return}
hist.lastOrigin=hist.lastSelOrigin=null
for(;;){event=source.pop()
if(event.ranges){pushSelectionToHistory(event,dest)
if(allowSelectionOnly&&!event.equals(doc.sel)){setSelection(doc,event,{clearRedo:false})
return}
selAfter=event}
else{break}}
var antiChanges=[]
pushSelectionToHistory(selAfter,dest)
dest.push({changes:antiChanges,generation:hist.generation})
hist.generation=event.generation||++hist.maxGeneration
var filter=hasHandler(doc,"beforeChange")||doc.cm&&hasHandler(doc.cm,"beforeChange")
var loop=function(i){var change=event.changes[i]
change.origin=type
if(filter&&!filterChange(doc,change,false)){source.length=0
return{}}
antiChanges.push(historyChangeFromChange(doc,change))
var after=i?computeSelAfterChange(doc,change):lst(source)
makeChangeSingleDoc(doc,change,after,mergeOldSpans(doc,change))
if(!i&&doc.cm){doc.cm.scrollIntoView({from:change.from,to:changeEnd(change)})}
var rebased=[]
linkedDocs(doc,function(doc,sharedHist){if(!sharedHist&&indexOf(rebased,doc.history)==-1){rebaseHist(doc.history,change)
rebased.push(doc.history)}
makeChangeSingleDoc(doc,change,null,mergeOldSpans(doc,change))})};for(var i$1=event.changes.length-1;i$1>=0;--i$1){var returned=loop(i$1);if(returned)return returned.v;}}
function shiftDoc(doc,distance){if(distance==0){return}
doc.first+=distance
doc.sel=new Selection(map(doc.sel.ranges,function(range){return new Range(Pos(range.anchor.line+distance,range.anchor.ch),Pos(range.head.line+distance,range.head.ch));}),doc.sel.primIndex)
if(doc.cm){regChange(doc.cm,doc.first,doc.first-distance,distance)
for(var d=doc.cm.display,l=d.viewFrom;l<d.viewTo;l++)
{regLineChange(doc.cm,l,"gutter")}}}
function makeChangeSingleDoc(doc,change,selAfter,spans){if(doc.cm&&!doc.cm.curOp)
{return operation(doc.cm,makeChangeSingleDoc)(doc,change,selAfter,spans)}
if(change.to.line<doc.first){shiftDoc(doc,change.text.length-1-(change.to.line-change.from.line))
return}
if(change.from.line>doc.lastLine()){return}
if(change.from.line<doc.first){var shift=change.text.length-1-(doc.first-change.from.line)
shiftDoc(doc,shift)
change={from:Pos(doc.first,0),to:Pos(change.to.line+shift,change.to.ch),text:[lst(change.text)],origin:change.origin}}
var last=doc.lastLine()
if(change.to.line>last){change={from:change.from,to:Pos(last,getLine(doc,last).text.length),text:[change.text[0]],origin:change.origin}}
change.removed=getBetween(doc,change.from,change.to)
if(!selAfter){selAfter=computeSelAfterChange(doc,change)}
if(doc.cm){makeChangeSingleDocInEditor(doc.cm,change,spans)}
else{updateDoc(doc,change,spans)}
setSelectionNoUndo(doc,selAfter,sel_dontScroll)}
function makeChangeSingleDocInEditor(cm,change,spans){var doc=cm.doc,display=cm.display,from=change.from,to=change.to
var recomputeMaxLength=false,checkWidthStart=from.line
if(!cm.options.lineWrapping){checkWidthStart=lineNo(visualLine(getLine(doc,from.line)))
doc.iter(checkWidthStart,to.line+1,function(line){if(line==display.maxLine){recomputeMaxLength=true
return true}})}
if(doc.sel.contains(change.from,change.to)>-1)
{signalCursorActivity(cm)}
updateDoc(doc,change,spans,estimateHeight(cm))
if(!cm.options.lineWrapping){doc.iter(checkWidthStart,from.line+change.text.length,function(line){var len=lineLength(line)
if(len>display.maxLineLength){display.maxLine=line
display.maxLineLength=len
display.maxLineChanged=true
recomputeMaxLength=false}})
if(recomputeMaxLength){cm.curOp.updateMaxLine=true}}
doc.frontier=Math.min(doc.frontier,from.line)
startWorker(cm,400)
var lendiff=change.text.length-(to.line-from.line)-1
if(change.full)
{regChange(cm)}
else if(from.line==to.line&&change.text.length==1&&!isWholeLineUpdate(cm.doc,change))
{regLineChange(cm,from.line,"text")}
else
{regChange(cm,from.line,to.line+1,lendiff)}
var changesHandler=hasHandler(cm,"changes"),changeHandler=hasHandler(cm,"change")
if(changeHandler||changesHandler){var obj={from:from,to:to,text:change.text,removed:change.removed,origin:change.origin}
if(changeHandler){signalLater(cm,"change",cm,obj)}
if(changesHandler){(cm.curOp.changeObjs||(cm.curOp.changeObjs=[])).push(obj)}}
cm.display.selForContextMenu=null}
function replaceRange(doc,code,from,to,origin){if(!to){to=from}
if(cmp(to,from)<0){var tmp=to;to=from;from=tmp}
if(typeof code=="string"){code=doc.splitLines(code)}
makeChange(doc,{from:from,to:to,text:code,origin:origin})}
function rebaseHistSelSingle(pos,from,to,diff){if(to<pos.line){pos.line+=diff}else if(from<pos.line){pos.line=from
pos.ch=0}}
function rebaseHistArray(array,from,to,diff){for(var i=0;i<array.length;++i){var sub=array[i],ok=true
if(sub.ranges){if(!sub.copied){sub=array[i]=sub.deepCopy();sub.copied=true}
for(var j=0;j<sub.ranges.length;j++){rebaseHistSelSingle(sub.ranges[j].anchor,from,to,diff)
rebaseHistSelSingle(sub.ranges[j].head,from,to,diff)}
continue}
for(var j$1=0;j$1<sub.changes.length;++j$1){var cur=sub.changes[j$1]
if(to<cur.from.line){cur.from=Pos(cur.from.line+diff,cur.from.ch)
cur.to=Pos(cur.to.line+diff,cur.to.ch)}else if(from<=cur.to.line){ok=false
break}}
if(!ok){array.splice(0,i+1)
i=0}}}
function rebaseHist(hist,change){var from=change.from.line,to=change.to.line,diff=change.text.length-(to-from)-1
rebaseHistArray(hist.done,from,to,diff)
rebaseHistArray(hist.undone,from,to,diff)}
function changeLine(doc,handle,changeType,op){var no=handle,line=handle
if(typeof handle=="number"){line=getLine(doc,clipLine(doc,handle))}
else{no=lineNo(handle)}
if(no==null){return null}
if(op(line,no)&&doc.cm){regLineChange(doc.cm,no,changeType)}
return line}
var LeafChunk=function LeafChunk(lines){var this$1=this;this.lines=lines
this.parent=null
var height=0
for(var i=0;i<lines.length;++i){lines[i].parent=this$1
height+=lines[i].height}
this.height=height};LeafChunk.prototype.chunkSize=function chunkSize(){return this.lines.length};LeafChunk.prototype.removeInner=function removeInner(at,n){var this$1=this;for(var i=at,e=at+n;i<e;++i){var line=this$1.lines[i]
this$1.height-=line.height
cleanUpLine(line)
signalLater(line,"delete")}
this.lines.splice(at,n)};LeafChunk.prototype.collapse=function collapse(lines){lines.push.apply(lines,this.lines)};LeafChunk.prototype.insertInner=function insertInner(at,lines,height){var this$1=this;this.height+=height
this.lines=this.lines.slice(0,at).concat(lines).concat(this.lines.slice(at))
for(var i=0;i<lines.length;++i){lines[i].parent=this$1}};LeafChunk.prototype.iterN=function iterN(at,n,op){var this$1=this;for(var e=at+n;at<e;++at)
{if(op(this$1.lines[at])){return true}}};var BranchChunk=function BranchChunk(children){var this$1=this;this.children=children
var size=0,height=0
for(var i=0;i<children.length;++i){var ch=children[i]
size+=ch.chunkSize();height+=ch.height
ch.parent=this$1}
this.size=size
this.height=height
this.parent=null};BranchChunk.prototype.chunkSize=function chunkSize(){return this.size};BranchChunk.prototype.removeInner=function removeInner(at,n){var this$1=this;this.size-=n
for(var i=0;i<this.children.length;++i){var child=this$1.children[i],sz=child.chunkSize()
if(at<sz){var rm=Math.min(n,sz-at),oldHeight=child.height
child.removeInner(at,rm)
this$1.height-=oldHeight-child.height
if(sz==rm){this$1.children.splice(i--,1);child.parent=null}
if((n-=rm)==0){break}
at=0}else{at-=sz}}
if(this.size-n<25&&(this.children.length>1||!(this.children[0]instanceof LeafChunk))){var lines=[]
this.collapse(lines)
this.children=[new LeafChunk(lines)]
this.children[0].parent=this}};BranchChunk.prototype.collapse=function collapse(lines){var this$1=this;for(var i=0;i<this.children.length;++i){this$1.children[i].collapse(lines)}};BranchChunk.prototype.insertInner=function insertInner(at,lines,height){var this$1=this;this.size+=lines.length
this.height+=height
for(var i=0;i<this.children.length;++i){var child=this$1.children[i],sz=child.chunkSize()
if(at<=sz){child.insertInner(at,lines,height)
if(child.lines&&child.lines.length>50){var remaining=child.lines.length%25+25
for(var pos=remaining;pos<child.lines.length;){var leaf=new LeafChunk(child.lines.slice(pos,pos+=25))
child.height-=leaf.height
this$1.children.splice(++i,0,leaf)
leaf.parent=this$1}
child.lines=child.lines.slice(0,remaining)
this$1.maybeSpill()}
break}
at-=sz}};BranchChunk.prototype.maybeSpill=function maybeSpill(){if(this.children.length<=10){return}
var me=this
do{var spilled=me.children.splice(me.children.length-5,5)
var sibling=new BranchChunk(spilled)
if(!me.parent){var copy=new BranchChunk(me.children)
copy.parent=me
me.children=[copy,sibling]
me=copy}else{me.size-=sibling.size
me.height-=sibling.height
var myIndex=indexOf(me.parent.children,me)
me.parent.children.splice(myIndex+1,0,sibling)}
sibling.parent=me.parent}while(me.children.length>10)
me.parent.maybeSpill()};BranchChunk.prototype.iterN=function iterN(at,n,op){var this$1=this;for(var i=0;i<this.children.length;++i){var child=this$1.children[i],sz=child.chunkSize()
if(at<sz){var used=Math.min(n,sz-at)
if(child.iterN(at,used,op)){return true}
if((n-=used)==0){break}
at=0}else{at-=sz}}};var LineWidget=function LineWidget(doc,node,options){var this$1=this;if(options){for(var opt in options){if(options.hasOwnProperty(opt))
{this$1[opt]=options[opt]}}}
this.doc=doc
this.node=node};LineWidget.prototype.clear=function clear(){var this$1=this;var cm=this.doc.cm,ws=this.line.widgets,line=this.line,no=lineNo(line)
if(no==null||!ws){return}
for(var i=0;i<ws.length;++i){if(ws[i]==this$1){ws.splice(i--,1)}}
if(!ws.length){line.widgets=null}
var height=widgetHeight(this)
updateLineHeight(line,Math.max(0,line.height-height))
if(cm){runInOp(cm,function(){adjustScrollWhenAboveVisible(cm,line,-height)
regLineChange(cm,no,"widget")})
signalLater(cm,"lineWidgetCleared",cm,this,no)}};LineWidget.prototype.changed=function changed(){var this$1=this;var oldH=this.height,cm=this.doc.cm,line=this.line
this.height=null
var diff=widgetHeight(this)-oldH
if(!diff){return}
updateLineHeight(line,line.height+diff)
if(cm){runInOp(cm,function(){cm.curOp.forceUpdate=true
adjustScrollWhenAboveVisible(cm,line,diff)
signalLater(cm,"lineWidgetChanged",cm,this$1,lineNo(line))})}};eventMixin(LineWidget)
function adjustScrollWhenAboveVisible(cm,line,diff){if(heightAtLine(line)<((cm.curOp&&cm.curOp.scrollTop)||cm.doc.scrollTop))
{addToScrollPos(cm,null,diff)}}
function addLineWidget(doc,handle,node,options){var widget=new LineWidget(doc,node,options)
var cm=doc.cm
if(cm&&widget.noHScroll){cm.display.alignWidgets=true}
changeLine(doc,handle,"widget",function(line){var widgets=line.widgets||(line.widgets=[])
if(widget.insertAt==null){widgets.push(widget)}
else{widgets.splice(Math.min(widgets.length-1,Math.max(0,widget.insertAt)),0,widget)}
widget.line=line
if(cm&&!lineIsHidden(doc,line)){var aboveVisible=heightAtLine(line)<doc.scrollTop
updateLineHeight(line,line.height+widgetHeight(widget))
if(aboveVisible){addToScrollPos(cm,null,widget.height)}
cm.curOp.forceUpdate=true}
return true})
signalLater(cm,"lineWidgetAdded",cm,widget,typeof handle=="number"?handle:lineNo(handle))
return widget}
var nextMarkerId=0
var TextMarker=function TextMarker(doc,type){this.lines=[]
this.type=type
this.doc=doc
this.id=++nextMarkerId};TextMarker.prototype.clear=function clear(){var this$1=this;if(this.explicitlyCleared){return}
var cm=this.doc.cm,withOp=cm&&!cm.curOp
if(withOp){startOperation(cm)}
if(hasHandler(this,"clear")){var found=this.find()
if(found){signalLater(this,"clear",found.from,found.to)}}
var min=null,max=null
for(var i=0;i<this.lines.length;++i){var line=this$1.lines[i]
var span=getMarkedSpanFor(line.markedSpans,this$1)
if(cm&&!this$1.collapsed){regLineChange(cm,lineNo(line),"text")}
else if(cm){if(span.to!=null){max=lineNo(line)}
if(span.from!=null){min=lineNo(line)}}
line.markedSpans=removeMarkedSpan(line.markedSpans,span)
if(span.from==null&&this$1.collapsed&&!lineIsHidden(this$1.doc,line)&&cm)
{updateLineHeight(line,textHeight(cm.display))}}
if(cm&&this.collapsed&&!cm.options.lineWrapping){for(var i$1=0;i$1<this.lines.length;++i$1){var visual=visualLine(this$1.lines[i$1]),len=lineLength(visual)
if(len>cm.display.maxLineLength){cm.display.maxLine=visual
cm.display.maxLineLength=len
cm.display.maxLineChanged=true}}}
if(min!=null&&cm&&this.collapsed){regChange(cm,min,max+1)}
this.lines.length=0
this.explicitlyCleared=true
if(this.atomic&&this.doc.cantEdit){this.doc.cantEdit=false
if(cm){reCheckSelection(cm.doc)}}
if(cm){signalLater(cm,"markerCleared",cm,this,min,max)}
if(withOp){endOperation(cm)}
if(this.parent){this.parent.clear()}};TextMarker.prototype.find=function find(side,lineObj){var this$1=this;if(side==null&&this.type=="bookmark"){side=1}
var from,to
for(var i=0;i<this.lines.length;++i){var line=this$1.lines[i]
var span=getMarkedSpanFor(line.markedSpans,this$1)
if(span.from!=null){from=Pos(lineObj?line:lineNo(line),span.from)
if(side==-1){return from}}
if(span.to!=null){to=Pos(lineObj?line:lineNo(line),span.to)
if(side==1){return to}}}
return from&&{from:from,to:to}};TextMarker.prototype.changed=function changed(){var this$1=this;var pos=this.find(-1,true),widget=this,cm=this.doc.cm
if(!pos||!cm){return}
runInOp(cm,function(){var line=pos.line,lineN=lineNo(pos.line)
var view=findViewForLine(cm,lineN)
if(view){clearLineMeasurementCacheFor(view)
cm.curOp.selectionChanged=cm.curOp.forceUpdate=true}
cm.curOp.updateMaxLine=true
if(!lineIsHidden(widget.doc,line)&&widget.height!=null){var oldHeight=widget.height
widget.height=null
var dHeight=widgetHeight(widget)-oldHeight
if(dHeight)
{updateLineHeight(line,line.height+dHeight)}}
signalLater(cm,"markerChanged",cm,this$1)})};TextMarker.prototype.attachLine=function attachLine(line){if(!this.lines.length&&this.doc.cm){var op=this.doc.cm.curOp
if(!op.maybeHiddenMarkers||indexOf(op.maybeHiddenMarkers,this)==-1)
{(op.maybeUnhiddenMarkers||(op.maybeUnhiddenMarkers=[])).push(this)}}
this.lines.push(line)};TextMarker.prototype.detachLine=function detachLine(line){this.lines.splice(indexOf(this.lines,line),1)
if(!this.lines.length&&this.doc.cm){var op=this.doc.cm.curOp;(op.maybeHiddenMarkers||(op.maybeHiddenMarkers=[])).push(this)}};eventMixin(TextMarker)
function markText(doc,from,to,options,type){if(options&&options.shared){return markTextShared(doc,from,to,options,type)}
if(doc.cm&&!doc.cm.curOp){return operation(doc.cm,markText)(doc,from,to,options,type)}
var marker=new TextMarker(doc,type),diff=cmp(from,to)
if(options){copyObj(options,marker,false)}
if(diff>0||diff==0&&marker.clearWhenEmpty!==false)
{return marker}
if(marker.replacedWith){marker.collapsed=true
marker.widgetNode=eltP("span",[marker.replacedWith],"CodeMirror-widget")
if(!options.handleMouseEvents){marker.widgetNode.setAttribute("cm-ignore-events","true")}
if(options.insertLeft){marker.widgetNode.insertLeft=true}}
if(marker.collapsed){if(conflictingCollapsedRange(doc,from.line,from,to,marker)||from.line!=to.line&&conflictingCollapsedRange(doc,to.line,from,to,marker))
{throw new Error("Inserting collapsed marker partially overlapping an existing one")}
seeCollapsedSpans()}
if(marker.addToHistory)
{addChangeToHistory(doc,{from:from,to:to,origin:"markText"},doc.sel,NaN)}
var curLine=from.line,cm=doc.cm,updateMaxLine
doc.iter(curLine,to.line+1,function(line){if(cm&&marker.collapsed&&!cm.options.lineWrapping&&visualLine(line)==cm.display.maxLine)
{updateMaxLine=true}
if(marker.collapsed&&curLine!=from.line){updateLineHeight(line,0)}
addMarkedSpan(line,new MarkedSpan(marker,curLine==from.line?from.ch:null,curLine==to.line?to.ch:null))
++curLine})
if(marker.collapsed){doc.iter(from.line,to.line+1,function(line){if(lineIsHidden(doc,line)){updateLineHeight(line,0)}})}
if(marker.clearOnEnter){on(marker,"beforeCursorEnter",function(){return marker.clear();})}
if(marker.readOnly){seeReadOnlySpans()
if(doc.history.done.length||doc.history.undone.length)
{doc.clearHistory()}}
if(marker.collapsed){marker.id=++nextMarkerId
marker.atomic=true}
if(cm){if(updateMaxLine){cm.curOp.updateMaxLine=true}
if(marker.collapsed)
{regChange(cm,from.line,to.line+1)}
else if(marker.className||marker.title||marker.startStyle||marker.endStyle||marker.css)
{for(var i=from.line;i<=to.line;i++){regLineChange(cm,i,"text")}}
if(marker.atomic){reCheckSelection(cm.doc)}
signalLater(cm,"markerAdded",cm,marker)}
return marker}
var SharedTextMarker=function SharedTextMarker(markers,primary){var this$1=this;this.markers=markers
this.primary=primary
for(var i=0;i<markers.length;++i)
{markers[i].parent=this$1}};SharedTextMarker.prototype.clear=function clear(){var this$1=this;if(this.explicitlyCleared){return}
this.explicitlyCleared=true
for(var i=0;i<this.markers.length;++i)
{this$1.markers[i].clear()}
signalLater(this,"clear")};SharedTextMarker.prototype.find=function find(side,lineObj){return this.primary.find(side,lineObj)};eventMixin(SharedTextMarker)
function markTextShared(doc,from,to,options,type){options=copyObj(options)
options.shared=false
var markers=[markText(doc,from,to,options,type)],primary=markers[0]
var widget=options.widgetNode
linkedDocs(doc,function(doc){if(widget){options.widgetNode=widget.cloneNode(true)}
markers.push(markText(doc,clipPos(doc,from),clipPos(doc,to),options,type))
for(var i=0;i<doc.linked.length;++i)
{if(doc.linked[i].isParent){return}}
primary=lst(markers)})
return new SharedTextMarker(markers,primary)}
function findSharedMarkers(doc){return doc.findMarks(Pos(doc.first,0),doc.clipPos(Pos(doc.lastLine())),function(m){return m.parent;})}
function copySharedMarkers(doc,markers){for(var i=0;i<markers.length;i++){var marker=markers[i],pos=marker.find()
var mFrom=doc.clipPos(pos.from),mTo=doc.clipPos(pos.to)
if(cmp(mFrom,mTo)){var subMark=markText(doc,mFrom,mTo,marker.primary,marker.primary.type)
marker.markers.push(subMark)
subMark.parent=marker}}}
function detachSharedMarkers(markers){var loop=function(i){var marker=markers[i],linked=[marker.primary.doc]
linkedDocs(marker.primary.doc,function(d){return linked.push(d);})
for(var j=0;j<marker.markers.length;j++){var subMarker=marker.markers[j]
if(indexOf(linked,subMarker.doc)==-1){subMarker.parent=null
marker.markers.splice(j--,1)}}};for(var i=0;i<markers.length;i++)loop(i);}
var nextDocId=0
var Doc=function(text,mode,firstLine,lineSep,direction){if(!(this instanceof Doc)){return new Doc(text,mode,firstLine,lineSep,direction)}
if(firstLine==null){firstLine=0}
BranchChunk.call(this,[new LeafChunk([new Line("",null)])])
this.first=firstLine
this.scrollTop=this.scrollLeft=0
this.cantEdit=false
this.cleanGeneration=1
this.frontier=firstLine
var start=Pos(firstLine,0)
this.sel=simpleSelection(start)
this.history=new History(null)
this.id=++nextDocId
this.modeOption=mode
this.lineSep=lineSep
this.direction=(direction=="rtl")?"rtl":"ltr"
this.extend=false
if(typeof text=="string"){text=this.splitLines(text)}
updateDoc(this,{from:start,to:start,text:text})
setSelection(this,simpleSelection(start),sel_dontScroll)}
Doc.prototype=createObj(BranchChunk.prototype,{constructor:Doc,iter:function(from,to,op){if(op){this.iterN(from-this.first,to-from,op)}
else{this.iterN(this.first,this.first+this.size,from)}},insert:function(at,lines){var height=0
for(var i=0;i<lines.length;++i){height+=lines[i].height}
this.insertInner(at-this.first,lines,height)},remove:function(at,n){this.removeInner(at-this.first,n)},getValue:function(lineSep){var lines=getLines(this,this.first,this.first+this.size)
if(lineSep===false){return lines}
return lines.join(lineSep||this.lineSeparator())},setValue:docMethodOp(function(code){var top=Pos(this.first,0),last=this.first+this.size-1
makeChange(this,{from:top,to:Pos(last,getLine(this,last).text.length),text:this.splitLines(code),origin:"setValue",full:true},true)
setSelection(this,simpleSelection(top))}),replaceRange:function(code,from,to,origin){from=clipPos(this,from)
to=to?clipPos(this,to):from
replaceRange(this,code,from,to,origin)},getRange:function(from,to,lineSep){var lines=getBetween(this,clipPos(this,from),clipPos(this,to))
if(lineSep===false){return lines}
return lines.join(lineSep||this.lineSeparator())},getLine:function(line){var l=this.getLineHandle(line);return l&&l.text},getLineHandle:function(line){if(isLine(this,line)){return getLine(this,line)}},getLineNumber:function(line){return lineNo(line)},getLineHandleVisualStart:function(line){if(typeof line=="number"){line=getLine(this,line)}
return visualLine(line)},lineCount:function(){return this.size},firstLine:function(){return this.first},lastLine:function(){return this.first+this.size-1},clipPos:function(pos){return clipPos(this,pos)},getCursor:function(start){var range=this.sel.primary(),pos
if(start==null||start=="head"){pos=range.head}
else if(start=="anchor"){pos=range.anchor}
else if(start=="end"||start=="to"||start===false){pos=range.to()}
else{pos=range.from()}
return pos},listSelections:function(){return this.sel.ranges},somethingSelected:function(){return this.sel.somethingSelected()},setCursor:docMethodOp(function(line,ch,options){setSimpleSelection(this,clipPos(this,typeof line=="number"?Pos(line,ch||0):line),null,options)}),setSelection:docMethodOp(function(anchor,head,options){setSimpleSelection(this,clipPos(this,anchor),clipPos(this,head||anchor),options)}),extendSelection:docMethodOp(function(head,other,options){extendSelection(this,clipPos(this,head),other&&clipPos(this,other),options)}),extendSelections:docMethodOp(function(heads,options){extendSelections(this,clipPosArray(this,heads),options)}),extendSelectionsBy:docMethodOp(function(f,options){var heads=map(this.sel.ranges,f)
extendSelections(this,clipPosArray(this,heads),options)}),setSelections:docMethodOp(function(ranges,primary,options){var this$1=this;if(!ranges.length){return}
var out=[]
for(var i=0;i<ranges.length;i++)
{out[i]=new Range(clipPos(this$1,ranges[i].anchor),clipPos(this$1,ranges[i].head))}
if(primary==null){primary=Math.min(ranges.length-1,this.sel.primIndex)}
setSelection(this,normalizeSelection(out,primary),options)}),addSelection:docMethodOp(function(anchor,head,options){var ranges=this.sel.ranges.slice(0)
ranges.push(new Range(clipPos(this,anchor),clipPos(this,head||anchor)))
setSelection(this,normalizeSelection(ranges,ranges.length-1),options)}),getSelection:function(lineSep){var this$1=this;var ranges=this.sel.ranges,lines
for(var i=0;i<ranges.length;i++){var sel=getBetween(this$1,ranges[i].from(),ranges[i].to())
lines=lines?lines.concat(sel):sel}
if(lineSep===false){return lines}
else{return lines.join(lineSep||this.lineSeparator())}},getSelections:function(lineSep){var this$1=this;var parts=[],ranges=this.sel.ranges
for(var i=0;i<ranges.length;i++){var sel=getBetween(this$1,ranges[i].from(),ranges[i].to())
if(lineSep!==false){sel=sel.join(lineSep||this$1.lineSeparator())}
parts[i]=sel}
return parts},replaceSelection:function(code,collapse,origin){var dup=[]
for(var i=0;i<this.sel.ranges.length;i++)
{dup[i]=code}
this.replaceSelections(dup,collapse,origin||"+input")},replaceSelections:docMethodOp(function(code,collapse,origin){var this$1=this;var changes=[],sel=this.sel
for(var i=0;i<sel.ranges.length;i++){var range=sel.ranges[i]
changes[i]={from:range.from(),to:range.to(),text:this$1.splitLines(code[i]),origin:origin}}
var newSel=collapse&&collapse!="end"&&computeReplacedSel(this,changes,collapse)
for(var i$1=changes.length-1;i$1>=0;i$1--)
{makeChange(this$1,changes[i$1])}
if(newSel){setSelectionReplaceHistory(this,newSel)}
else if(this.cm){ensureCursorVisible(this.cm)}}),undo:docMethodOp(function(){makeChangeFromHistory(this,"undo")}),redo:docMethodOp(function(){makeChangeFromHistory(this,"redo")}),undoSelection:docMethodOp(function(){makeChangeFromHistory(this,"undo",true)}),redoSelection:docMethodOp(function(){makeChangeFromHistory(this,"redo",true)}),setExtending:function(val){this.extend=val},getExtending:function(){return this.extend},historySize:function(){var hist=this.history,done=0,undone=0
for(var i=0;i<hist.done.length;i++){if(!hist.done[i].ranges){++done}}
for(var i$1=0;i$1<hist.undone.length;i$1++){if(!hist.undone[i$1].ranges){++undone}}
return{undo:done,redo:undone}},clearHistory:function(){this.history=new History(this.history.maxGeneration)},markClean:function(){this.cleanGeneration=this.changeGeneration(true)},changeGeneration:function(forceSplit){if(forceSplit)
{this.history.lastOp=this.history.lastSelOp=this.history.lastOrigin=null}
return this.history.generation},isClean:function(gen){return this.history.generation==(gen||this.cleanGeneration)},getHistory:function(){return{done:copyHistoryArray(this.history.done),undone:copyHistoryArray(this.history.undone)}},setHistory:function(histData){var hist=this.history=new History(this.history.maxGeneration)
hist.done=copyHistoryArray(histData.done.slice(0),null,true)
hist.undone=copyHistoryArray(histData.undone.slice(0),null,true)},setGutterMarker:docMethodOp(function(line,gutterID,value){return changeLine(this,line,"gutter",function(line){var markers=line.gutterMarkers||(line.gutterMarkers={})
markers[gutterID]=value
if(!value&&isEmpty(markers)){line.gutterMarkers=null}
return true})}),clearGutter:docMethodOp(function(gutterID){var this$1=this;this.iter(function(line){if(line.gutterMarkers&&line.gutterMarkers[gutterID]){changeLine(this$1,line,"gutter",function(){line.gutterMarkers[gutterID]=null
if(isEmpty(line.gutterMarkers)){line.gutterMarkers=null}
return true})}})}),lineInfo:function(line){var n
if(typeof line=="number"){if(!isLine(this,line)){return null}
n=line
line=getLine(this,line)
if(!line){return null}}else{n=lineNo(line)
if(n==null){return null}}
return{line:n,handle:line,text:line.text,gutterMarkers:line.gutterMarkers,textClass:line.textClass,bgClass:line.bgClass,wrapClass:line.wrapClass,widgets:line.widgets}},addLineClass:docMethodOp(function(handle,where,cls){return changeLine(this,handle,where=="gutter"?"gutter":"class",function(line){var prop=where=="text"?"textClass":where=="background"?"bgClass":where=="gutter"?"gutterClass":"wrapClass"
if(!line[prop]){line[prop]=cls}
else if(classTest(cls).test(line[prop])){return false}
else{line[prop]+=" "+cls}
return true})}),removeLineClass:docMethodOp(function(handle,where,cls){return changeLine(this,handle,where=="gutter"?"gutter":"class",function(line){var prop=where=="text"?"textClass":where=="background"?"bgClass":where=="gutter"?"gutterClass":"wrapClass"
var cur=line[prop]
if(!cur){return false}
else if(cls==null){line[prop]=null}
else{var found=cur.match(classTest(cls))
if(!found){return false}
var end=found.index+found[0].length
line[prop]=cur.slice(0,found.index)+(!found.index||end==cur.length?"":" ")+cur.slice(end)||null}
return true})}),addLineWidget:docMethodOp(function(handle,node,options){return addLineWidget(this,handle,node,options)}),removeLineWidget:function(widget){widget.clear()},markText:function(from,to,options){return markText(this,clipPos(this,from),clipPos(this,to),options,options&&options.type||"range")},setBookmark:function(pos,options){var realOpts={replacedWith:options&&(options.nodeType==null?options.widget:options),insertLeft:options&&options.insertLeft,clearWhenEmpty:false,shared:options&&options.shared,handleMouseEvents:options&&options.handleMouseEvents}
pos=clipPos(this,pos)
return markText(this,pos,pos,realOpts,"bookmark")},findMarksAt:function(pos){pos=clipPos(this,pos)
var markers=[],spans=getLine(this,pos.line).markedSpans
if(spans){for(var i=0;i<spans.length;++i){var span=spans[i]
if((span.from==null||span.from<=pos.ch)&&(span.to==null||span.to>=pos.ch))
{markers.push(span.marker.parent||span.marker)}}}
return markers},findMarks:function(from,to,filter){from=clipPos(this,from);to=clipPos(this,to)
var found=[],lineNo=from.line
this.iter(from.line,to.line+1,function(line){var spans=line.markedSpans
if(spans){for(var i=0;i<spans.length;i++){var span=spans[i]
if(!(span.to!=null&&lineNo==from.line&&from.ch>=span.to||span.from==null&&lineNo!=from.line||span.from!=null&&lineNo==to.line&&span.from>=to.ch)&&(!filter||filter(span.marker)))
{found.push(span.marker.parent||span.marker)}}}
++lineNo})
return found},getAllMarks:function(){var markers=[]
this.iter(function(line){var sps=line.markedSpans
if(sps){for(var i=0;i<sps.length;++i)
{if(sps[i].from!=null){markers.push(sps[i].marker)}}}})
return markers},posFromIndex:function(off){var ch,lineNo=this.first,sepSize=this.lineSeparator().length
this.iter(function(line){var sz=line.text.length+sepSize
if(sz>off){ch=off;return true}
off-=sz
++lineNo})
return clipPos(this,Pos(lineNo,ch))},indexFromPos:function(coords){coords=clipPos(this,coords)
var index=coords.ch
if(coords.line<this.first||coords.ch<0){return 0}
var sepSize=this.lineSeparator().length
this.iter(this.first,coords.line,function(line){index+=line.text.length+sepSize})
return index},copy:function(copyHistory){var doc=new Doc(getLines(this,this.first,this.first+this.size),this.modeOption,this.first,this.lineSep,this.direction)
doc.scrollTop=this.scrollTop;doc.scrollLeft=this.scrollLeft
doc.sel=this.sel
doc.extend=false
if(copyHistory){doc.history.undoDepth=this.history.undoDepth
doc.setHistory(this.getHistory())}
return doc},linkedDoc:function(options){if(!options){options={}}
var from=this.first,to=this.first+this.size
if(options.from!=null&&options.from>from){from=options.from}
if(options.to!=null&&options.to<to){to=options.to}
var copy=new Doc(getLines(this,from,to),options.mode||this.modeOption,from,this.lineSep,this.direction)
if(options.sharedHist){copy.history=this.history;}(this.linked||(this.linked=[])).push({doc:copy,sharedHist:options.sharedHist})
copy.linked=[{doc:this,isParent:true,sharedHist:options.sharedHist}]
copySharedMarkers(copy,findSharedMarkers(this))
return copy},unlinkDoc:function(other){var this$1=this;if(other instanceof CodeMirror){other=other.doc}
if(this.linked){for(var i=0;i<this.linked.length;++i){var link=this$1.linked[i]
if(link.doc!=other){continue}
this$1.linked.splice(i,1)
other.unlinkDoc(this$1)
detachSharedMarkers(findSharedMarkers(this$1))
break}}
if(other.history==this.history){var splitIds=[other.id]
linkedDocs(other,function(doc){return splitIds.push(doc.id);},true)
other.history=new History(null)
other.history.done=copyHistoryArray(this.history.done,splitIds)
other.history.undone=copyHistoryArray(this.history.undone,splitIds)}},iterLinkedDocs:function(f){linkedDocs(this,f)},getMode:function(){return this.mode},getEditor:function(){return this.cm},splitLines:function(str){if(this.lineSep){return str.split(this.lineSep)}
return splitLinesAuto(str)},lineSeparator:function(){return this.lineSep||"\n"},setDirection:docMethodOp(function(dir){if(dir!="rtl"){dir="ltr"}
if(dir==this.direction){return}
this.direction=dir
this.iter(function(line){return line.order=null;})
if(this.cm){directionChanged(this.cm)}})})
Doc.prototype.eachLine=Doc.prototype.iter
var lastDrop=0
function onDrop(e){var cm=this
clearDragCursor(cm)
if(signalDOMEvent(cm,e)||eventInWidget(cm.display,e))
{return}
e_preventDefault(e)
if(ie){lastDrop=+new Date}
var pos=posFromMouse(cm,e,true),files=e.dataTransfer.files
if(!pos||cm.isReadOnly()){return}
if(files&&files.length&&window.FileReader&&window.File){var n=files.length,text=Array(n),read=0
var loadFile=function(file,i){if(cm.options.allowDropFileTypes&&indexOf(cm.options.allowDropFileTypes,file.type)==-1)
{return}
var reader=new FileReader
reader.onload=operation(cm,function(){var content=reader.result
if(/[\x00-\x08\x0e-\x1f]{2}/.test(content)){content=""}
text[i]=content
if(++read==n){pos=clipPos(cm.doc,pos)
var change={from:pos,to:pos,text:cm.doc.splitLines(text.join(cm.doc.lineSeparator())),origin:"paste"}
makeChange(cm.doc,change)
setSelectionReplaceHistory(cm.doc,simpleSelection(pos,changeEnd(change)))}})
reader.readAsText(file)}
for(var i=0;i<n;++i){loadFile(files[i],i)}}else{if(cm.state.draggingText&&cm.doc.sel.contains(pos)>-1){cm.state.draggingText(e)
setTimeout(function(){return cm.display.input.focus();},20)
return}
try{var text$1=e.dataTransfer.getData("Text")
if(text$1){var selected
if(cm.state.draggingText&&!cm.state.draggingText.copy)
{selected=cm.listSelections()}
setSelectionNoUndo(cm.doc,simpleSelection(pos,pos))
if(selected){for(var i$1=0;i$1<selected.length;++i$1)
{replaceRange(cm.doc,"",selected[i$1].anchor,selected[i$1].head,"drag")}}
cm.replaceSelection(text$1,"around","paste")
cm.display.input.focus()}}
catch(e){}}}
function onDragStart(cm,e){if(ie&&(!cm.state.draggingText||+new Date-lastDrop<100)){e_stop(e);return}
if(signalDOMEvent(cm,e)||eventInWidget(cm.display,e)){return}
e.dataTransfer.setData("Text",cm.getSelection())
e.dataTransfer.effectAllowed="copyMove"
if(e.dataTransfer.setDragImage&&!safari){var img=elt("img",null,null,"position: fixed; left: 0; top: 0;")
img.src="data:image/gif;base64,R0lGODlhAQABAAAAACH5BAEKAAEALAAAAAABAAEAAAICTAEAOw=="
if(presto){img.width=img.height=1
cm.display.wrapper.appendChild(img)
img._top=img.offsetTop}
e.dataTransfer.setDragImage(img,0,0)
if(presto){img.parentNode.removeChild(img)}}}
function onDragOver(cm,e){var pos=posFromMouse(cm,e)
if(!pos){return}
var frag=document.createDocumentFragment()
drawSelectionCursor(cm,pos,frag)
if(!cm.display.dragCursor){cm.display.dragCursor=elt("div",null,"CodeMirror-cursors CodeMirror-dragcursors")
cm.display.lineSpace.insertBefore(cm.display.dragCursor,cm.display.cursorDiv)}
removeChildrenAndAdd(cm.display.dragCursor,frag)}
function clearDragCursor(cm){if(cm.display.dragCursor){cm.display.lineSpace.removeChild(cm.display.dragCursor)
cm.display.dragCursor=null}}
function forEachCodeMirror(f){if(!document.body.getElementsByClassName){return}
var byClass=document.body.getElementsByClassName("CodeMirror")
for(var i=0;i<byClass.length;i++){var cm=byClass[i].CodeMirror
if(cm){f(cm)}}}
var globalsRegistered=false
function ensureGlobalHandlers(){if(globalsRegistered){return}
registerGlobalHandlers()
globalsRegistered=true}
function registerGlobalHandlers(){var resizeTimer
on(window,"resize",function(){if(resizeTimer==null){resizeTimer=setTimeout(function(){resizeTimer=null
forEachCodeMirror(onResize)},100)}})
on(window,"blur",function(){return forEachCodeMirror(onBlur);})}
function onResize(cm){var d=cm.display
if(d.lastWrapHeight==d.wrapper.clientHeight&&d.lastWrapWidth==d.wrapper.clientWidth)
{return}
d.cachedCharWidth=d.cachedTextHeight=d.cachedPaddingH=null
d.scrollbarsClipped=false
cm.setSize()}
var keyNames={3:"Enter",8:"Backspace",9:"Tab",13:"Enter",16:"Shift",17:"Ctrl",18:"Alt",19:"Pause",20:"CapsLock",27:"Esc",32:"Space",33:"PageUp",34:"PageDown",35:"End",36:"Home",37:"Left",38:"Up",39:"Right",40:"Down",44:"PrintScrn",45:"Insert",46:"Delete",59:";",61:"=",91:"Mod",92:"Mod",93:"Mod",106:"*",107:"=",109:"-",110:".",111:"/",127:"Delete",173:"-",186:";",187:"=",188:",",189:"-",190:".",191:"/",192:"`",219:"[",220:"\\",221:"]",222:"'",63232:"Up",63233:"Down",63234:"Left",63235:"Right",63272:"Delete",63273:"Home",63275:"End",63276:"PageUp",63277:"PageDown",63302:"Insert"}
for(var i=0;i<10;i++){keyNames[i+48]=keyNames[i+96]=String(i)}
for(var i$1=65;i$1<=90;i$1++){keyNames[i$1]=String.fromCharCode(i$1)}
for(var i$2=1;i$2<=12;i$2++){keyNames[i$2+111]=keyNames[i$2+63235]="F"+i$2}
var keyMap={}
keyMap.basic={"Left":"goCharLeft","Right":"goCharRight","Up":"goLineUp","Down":"goLineDown","End":"goLineEnd","Home":"goLineStartSmart","PageUp":"goPageUp","PageDown":"goPageDown","Delete":"delCharAfter","Backspace":"delCharBefore","Shift-Backspace":"delCharBefore","Tab":"defaultTab","Shift-Tab":"indentAuto","Enter":"newlineAndIndent","Insert":"toggleOverwrite","Esc":"singleSelection"}
keyMap.pcDefault={"Ctrl-A":"selectAll","Ctrl-D":"deleteLine","Ctrl-Z":"undo","Shift-Ctrl-Z":"redo","Ctrl-Y":"redo","Ctrl-Home":"goDocStart","Ctrl-End":"goDocEnd","Ctrl-Up":"goLineUp","Ctrl-Down":"goLineDown","Ctrl-Left":"goGroupLeft","Ctrl-Right":"goGroupRight","Alt-Left":"goLineStart","Alt-Right":"goLineEnd","Ctrl-Backspace":"delGroupBefore","Ctrl-Delete":"delGroupAfter","Ctrl-S":"save","Ctrl-F":"find","Ctrl-G":"findNext","Shift-Ctrl-G":"findPrev","Shift-Ctrl-F":"replace","Shift-Ctrl-R":"replaceAll","Ctrl-[":"indentLess","Ctrl-]":"indentMore","Ctrl-U":"undoSelection","Shift-Ctrl-U":"redoSelection","Alt-U":"redoSelection",fallthrough:"basic"}
keyMap.emacsy={"Ctrl-F":"goCharRight","Ctrl-B":"goCharLeft","Ctrl-P":"goLineUp","Ctrl-N":"goLineDown","Alt-F":"goWordRight","Alt-B":"goWordLeft","Ctrl-A":"goLineStart","Ctrl-E":"goLineEnd","Ctrl-V":"goPageDown","Shift-Ctrl-V":"goPageUp","Ctrl-D":"delCharAfter","Ctrl-H":"delCharBefore","Alt-D":"delWordAfter","Alt-Backspace":"delWordBefore","Ctrl-K":"killLine","Ctrl-T":"transposeChars","Ctrl-O":"openLine"}
keyMap.macDefault={"Cmd-A":"selectAll","Cmd-D":"deleteLine","Cmd-Z":"undo","Shift-Cmd-Z":"redo","Cmd-Y":"redo","Cmd-Home":"goDocStart","Cmd-Up":"goDocStart","Cmd-End":"goDocEnd","Cmd-Down":"goDocEnd","Alt-Left":"goGroupLeft","Alt-Right":"goGroupRight","Cmd-Left":"goLineLeft","Cmd-Right":"goLineRight","Alt-Backspace":"delGroupBefore","Ctrl-Alt-Backspace":"delGroupAfter","Alt-Delete":"delGroupAfter","Cmd-S":"save","Cmd-F":"find","Cmd-G":"findNext","Shift-Cmd-G":"findPrev","Cmd-Alt-F":"replace","Shift-Cmd-Alt-F":"replaceAll","Cmd-[":"indentLess","Cmd-]":"indentMore","Cmd-Backspace":"delWrappedLineLeft","Cmd-Delete":"delWrappedLineRight","Cmd-U":"undoSelection","Shift-Cmd-U":"redoSelection","Ctrl-Up":"goDocStart","Ctrl-Down":"goDocEnd",fallthrough:["basic","emacsy"]}
keyMap["default"]=mac?keyMap.macDefault:keyMap.pcDefault
function normalizeKeyName(name){var parts=name.split(/-(?!$)/)
name=parts[parts.length-1]
var alt,ctrl,shift,cmd
for(var i=0;i<parts.length-1;i++){var mod=parts[i]
if(/^(cmd|meta|m)$/i.test(mod)){cmd=true}
else if(/^a(lt)?$/i.test(mod)){alt=true}
else if(/^(c|ctrl|control)$/i.test(mod)){ctrl=true}
else if(/^s(hift)?$/i.test(mod)){shift=true}
else{throw new Error("Unrecognized modifier name: "+mod)}}
if(alt){name="Alt-"+name}
if(ctrl){name="Ctrl-"+name}
if(cmd){name="Cmd-"+name}
if(shift){name="Shift-"+name}
return name}
function normalizeKeyMap(keymap){var copy={}
for(var keyname in keymap){if(keymap.hasOwnProperty(keyname)){var value=keymap[keyname]
if(/^(name|fallthrough|(de|at)tach)$/.test(keyname)){continue}
if(value=="..."){delete keymap[keyname];continue}
var keys=map(keyname.split(" "),normalizeKeyName)
for(var i=0;i<keys.length;i++){var val=(void 0),name=(void 0)
if(i==keys.length-1){name=keys.join(" ")
val=value}else{name=keys.slice(0,i+1).join(" ")
val="..."}
var prev=copy[name]
if(!prev){copy[name]=val}
else if(prev!=val){throw new Error("Inconsistent bindings for "+name)}}
delete keymap[keyname]}}
for(var prop in copy){keymap[prop]=copy[prop]}
return keymap}
function lookupKey(key,map,handle,context){map=getKeyMap(map)
var found=map.call?map.call(key,context):map[key]
if(found===false){return"nothing"}
if(found==="..."){return"multi"}
if(found!=null&&handle(found)){return"handled"}
if(map.fallthrough){if(Object.prototype.toString.call(map.fallthrough)!="[object Array]")
{return lookupKey(key,map.fallthrough,handle,context)}
for(var i=0;i<map.fallthrough.length;i++){var result=lookupKey(key,map.fallthrough[i],handle,context)
if(result){return result}}}}
function isModifierKey(value){var name=typeof value=="string"?value:keyNames[value.keyCode]
return name=="Ctrl"||name=="Alt"||name=="Shift"||name=="Mod"}
function keyName(event,noShift){if(presto&&event.keyCode==34&&event["char"]){return false}
var base=keyNames[event.keyCode],name=base
if(name==null||event.altGraphKey){return false}
if(event.altKey&&base!="Alt"){name="Alt-"+name}
if((flipCtrlCmd?event.metaKey:event.ctrlKey)&&base!="Ctrl"){name="Ctrl-"+name}
if((flipCtrlCmd?event.ctrlKey:event.metaKey)&&base!="Cmd"){name="Cmd-"+name}
if(!noShift&&event.shiftKey&&base!="Shift"){name="Shift-"+name}
return name}
function getKeyMap(val){return typeof val=="string"?keyMap[val]:val}
function deleteNearSelection(cm,compute){var ranges=cm.doc.sel.ranges,kill=[]
for(var i=0;i<ranges.length;i++){var toKill=compute(ranges[i])
while(kill.length&&cmp(toKill.from,lst(kill).to)<=0){var replaced=kill.pop()
if(cmp(replaced.from,toKill.from)<0){toKill.from=replaced.from
break}}
kill.push(toKill)}
runInOp(cm,function(){for(var i=kill.length-1;i>=0;i--)
{replaceRange(cm.doc,"",kill[i].from,kill[i].to,"+delete")}
ensureCursorVisible(cm)})}
var commands={selectAll:selectAll,singleSelection:function(cm){return cm.setSelection(cm.getCursor("anchor"),cm.getCursor("head"),sel_dontScroll);},killLine:function(cm){return deleteNearSelection(cm,function(range){if(range.empty()){var len=getLine(cm.doc,range.head.line).text.length
if(range.head.ch==len&&range.head.line<cm.lastLine())
{return{from:range.head,to:Pos(range.head.line+1,0)}}
else
{return{from:range.head,to:Pos(range.head.line,len)}}}else{return{from:range.from(),to:range.to()}}});},deleteLine:function(cm){return deleteNearSelection(cm,function(range){return({from:Pos(range.from().line,0),to:clipPos(cm.doc,Pos(range.to().line+1,0))});});},delLineLeft:function(cm){return deleteNearSelection(cm,function(range){return({from:Pos(range.from().line,0),to:range.from()});});},delWrappedLineLeft:function(cm){return deleteNearSelection(cm,function(range){var top=cm.charCoords(range.head,"div").top+5
var leftPos=cm.coordsChar({left:0,top:top},"div")
return{from:leftPos,to:range.from()}});},delWrappedLineRight:function(cm){return deleteNearSelection(cm,function(range){var top=cm.charCoords(range.head,"div").top+5
var rightPos=cm.coordsChar({left:cm.display.lineDiv.offsetWidth+100,top:top},"div")
return{from:range.from(),to:rightPos}});},undo:function(cm){return cm.undo();},redo:function(cm){return cm.redo();},undoSelection:function(cm){return cm.undoSelection();},redoSelection:function(cm){return cm.redoSelection();},goDocStart:function(cm){return cm.extendSelection(Pos(cm.firstLine(),0));},goDocEnd:function(cm){return cm.extendSelection(Pos(cm.lastLine()));},goLineStart:function(cm){return cm.extendSelectionsBy(function(range){return lineStart(cm,range.head.line);},{origin:"+move",bias:1});},goLineStartSmart:function(cm){return cm.extendSelectionsBy(function(range){return lineStartSmart(cm,range.head);},{origin:"+move",bias:1});},goLineEnd:function(cm){return cm.extendSelectionsBy(function(range){return lineEnd(cm,range.head.line);},{origin:"+move",bias:-1});},goLineRight:function(cm){return cm.extendSelectionsBy(function(range){var top=cm.charCoords(range.head,"div").top+5
return cm.coordsChar({left:cm.display.lineDiv.offsetWidth+100,top:top},"div")},sel_move);},goLineLeft:function(cm){return cm.extendSelectionsBy(function(range){var top=cm.charCoords(range.head,"div").top+5
return cm.coordsChar({left:0,top:top},"div")},sel_move);},goLineLeftSmart:function(cm){return cm.extendSelectionsBy(function(range){var top=cm.charCoords(range.head,"div").top+5
var pos=cm.coordsChar({left:0,top:top},"div")
if(pos.ch<cm.getLine(pos.line).search(/\S/)){return lineStartSmart(cm,range.head)}
return pos},sel_move);},goLineUp:function(cm){return cm.moveV(-1,"line");},goLineDown:function(cm){return cm.moveV(1,"line");},goPageUp:function(cm){return cm.moveV(-1,"page");},goPageDown:function(cm){return cm.moveV(1,"page");},goCharLeft:function(cm){return cm.moveH(-1,"char");},goCharRight:function(cm){return cm.moveH(1,"char");},goColumnLeft:function(cm){return cm.moveH(-1,"column");},goColumnRight:function(cm){return cm.moveH(1,"column");},goWordLeft:function(cm){return cm.moveH(-1,"word");},goGroupRight:function(cm){return cm.moveH(1,"group");},goGroupLeft:function(cm){return cm.moveH(-1,"group");},goWordRight:function(cm){return cm.moveH(1,"word");},delCharBefore:function(cm){return cm.deleteH(-1,"char");},delCharAfter:function(cm){return cm.deleteH(1,"char");},delWordBefore:function(cm){return cm.deleteH(-1,"word");},delWordAfter:function(cm){return cm.deleteH(1,"word");},delGroupBefore:function(cm){return cm.deleteH(-1,"group");},delGroupAfter:function(cm){return cm.deleteH(1,"group");},indentAuto:function(cm){return cm.indentSelection("smart");},indentMore:function(cm){return cm.indentSelection("add");},indentLess:function(cm){return cm.indentSelection("subtract");},insertTab:function(cm){return cm.replaceSelection("\t");},insertSoftTab:function(cm){var spaces=[],ranges=cm.listSelections(),tabSize=cm.options.tabSize
for(var i=0;i<ranges.length;i++){var pos=ranges[i].from()
var col=countColumn(cm.getLine(pos.line),pos.ch,tabSize)
spaces.push(spaceStr(tabSize-col%tabSize))}
cm.replaceSelections(spaces)},defaultTab:function(cm){if(cm.somethingSelected()){cm.indentSelection("add")}
else{cm.execCommand("insertTab")}},transposeChars:function(cm){return runInOp(cm,function(){var ranges=cm.listSelections(),newSel=[]
for(var i=0;i<ranges.length;i++){if(!ranges[i].empty()){continue}
var cur=ranges[i].head,line=getLine(cm.doc,cur.line).text
if(line){if(cur.ch==line.length){cur=new Pos(cur.line,cur.ch-1)}
if(cur.ch>0){cur=new Pos(cur.line,cur.ch+1)
cm.replaceRange(line.charAt(cur.ch-1)+line.charAt(cur.ch-2),Pos(cur.line,cur.ch-2),cur,"+transpose")}else if(cur.line>cm.doc.first){var prev=getLine(cm.doc,cur.line-1).text
if(prev){cur=new Pos(cur.line,1)
cm.replaceRange(line.charAt(0)+cm.doc.lineSeparator()+
prev.charAt(prev.length-1),Pos(cur.line-1,prev.length-1),cur,"+transpose")}}}
newSel.push(new Range(cur,cur))}
cm.setSelections(newSel)});},newlineAndIndent:function(cm){return runInOp(cm,function(){var sels=cm.listSelections()
for(var i=sels.length-1;i>=0;i--)
{cm.replaceRange(cm.doc.lineSeparator(),sels[i].anchor,sels[i].head,"+input")}
sels=cm.listSelections()
for(var i$1=0;i$1<sels.length;i$1++)
{cm.indentLine(sels[i$1].from().line,null,true)}
ensureCursorVisible(cm)});},openLine:function(cm){return cm.replaceSelection("\n","start");},toggleOverwrite:function(cm){return cm.toggleOverwrite();}}
function lineStart(cm,lineN){var line=getLine(cm.doc,lineN)
var visual=visualLine(line)
if(visual!=line){lineN=lineNo(visual)}
return endOfLine(true,cm,visual,lineN,1)}
function lineEnd(cm,lineN){var line=getLine(cm.doc,lineN)
var visual=visualLineEnd(line)
if(visual!=line){lineN=lineNo(visual)}
return endOfLine(true,cm,line,lineN,-1)}
function lineStartSmart(cm,pos){var start=lineStart(cm,pos.line)
var line=getLine(cm.doc,start.line)
var order=getOrder(line,cm.doc.direction)
if(!order||order[0].level==0){var firstNonWS=Math.max(0,line.text.search(/\S/))
var inWS=pos.line==start.line&&pos.ch<=firstNonWS&&pos.ch
return Pos(start.line,inWS?0:firstNonWS,start.sticky)}
return start}
function doHandleBinding(cm,bound,dropShift){if(typeof bound=="string"){bound=commands[bound]
if(!bound){return false}}
cm.display.input.ensurePolled()
var prevShift=cm.display.shift,done=false
try{if(cm.isReadOnly()){cm.state.suppressEdits=true}
if(dropShift){cm.display.shift=false}
done=bound(cm)!=Pass}finally{cm.display.shift=prevShift
cm.state.suppressEdits=false}
return done}
function lookupKeyForEditor(cm,name,handle){for(var i=0;i<cm.state.keyMaps.length;i++){var result=lookupKey(name,cm.state.keyMaps[i],handle,cm)
if(result){return result}}
return(cm.options.extraKeys&&lookupKey(name,cm.options.extraKeys,handle,cm))||lookupKey(name,cm.options.keyMap,handle,cm)}
var stopSeq=new Delayed
function dispatchKey(cm,name,e,handle){var seq=cm.state.keySeq
if(seq){if(isModifierKey(name)){return"handled"}
stopSeq.set(50,function(){if(cm.state.keySeq==seq){cm.state.keySeq=null
cm.display.input.reset()}})
name=seq+" "+name}
var result=lookupKeyForEditor(cm,name,handle)
if(result=="multi")
{cm.state.keySeq=name}
if(result=="handled")
{signalLater(cm,"keyHandled",cm,name,e)}
if(result=="handled"||result=="multi"){e_preventDefault(e)
restartBlink(cm)}
if(seq&&!result&&/\'$/.test(name)){e_preventDefault(e)
return true}
return!!result}
function handleKeyBinding(cm,e){var name=keyName(e,true)
if(!name){return false}
if(e.shiftKey&&!cm.state.keySeq){return dispatchKey(cm,"Shift-"+name,e,function(b){return doHandleBinding(cm,b,true);})||dispatchKey(cm,name,e,function(b){if(typeof b=="string"?/^go[A-Z]/.test(b):b.motion)
{return doHandleBinding(cm,b)}})}else{return dispatchKey(cm,name,e,function(b){return doHandleBinding(cm,b);})}}
function handleCharBinding(cm,e,ch){return dispatchKey(cm,"'"+ch+"'",e,function(b){return doHandleBinding(cm,b,true);})}
var lastStoppedKey=null
function onKeyDown(e){var cm=this
cm.curOp.focus=activeElt()
if(signalDOMEvent(cm,e)){return}
if(ie&&ie_version<11&&e.keyCode==27){e.returnValue=false}
var code=e.keyCode
cm.display.shift=code==16||e.shiftKey
var handled=handleKeyBinding(cm,e)
if(presto){lastStoppedKey=handled?code:null
if(!handled&&code==88&&!hasCopyEvent&&(mac?e.metaKey:e.ctrlKey))
{cm.replaceSelection("",null,"cut")}}
if(code==18&&!/\bCodeMirror-crosshair\b/.test(cm.display.lineDiv.className))
{showCrossHair(cm)}}
function showCrossHair(cm){var lineDiv=cm.display.lineDiv
addClass(lineDiv,"CodeMirror-crosshair")
function up(e){if(e.keyCode==18||!e.altKey){rmClass(lineDiv,"CodeMirror-crosshair")
off(document,"keyup",up)
off(document,"mouseover",up)}}
on(document,"keyup",up)
on(document,"mouseover",up)}
function onKeyUp(e){if(e.keyCode==16){this.doc.sel.shift=false}
signalDOMEvent(this,e)}
function onKeyPress(e){var cm=this
if(eventInWidget(cm.display,e)||signalDOMEvent(cm,e)||e.ctrlKey&&!e.altKey||mac&&e.metaKey){return}
var keyCode=e.keyCode,charCode=e.charCode
if(presto&&keyCode==lastStoppedKey){lastStoppedKey=null;e_preventDefault(e);return}
if((presto&&(!e.which||e.which<10))&&handleKeyBinding(cm,e)){return}
var ch=String.fromCharCode(charCode==null?keyCode:charCode)
if(ch=="\x08"){return}
if(handleCharBinding(cm,e,ch)){return}
cm.display.input.onKeyPress(e)}
function onMouseDown(e){var cm=this,display=cm.display
if(signalDOMEvent(cm,e)||display.activeTouch&&display.input.supportsTouch()){return}
display.input.ensurePolled()
display.shift=e.shiftKey
if(eventInWidget(display,e)){if(!webkit){display.scroller.draggable=false
setTimeout(function(){return display.scroller.draggable=true;},100)}
return}
if(clickInGutter(cm,e)){return}
var start=posFromMouse(cm,e)
window.focus()
switch(e_button(e)){case 1:if(cm.state.selectingText)
{cm.state.selectingText(e)}
else if(start)
{leftButtonDown(cm,e,start)}
else if(e_target(e)==display.scroller)
{e_preventDefault(e)}
break
case 2:if(webkit){cm.state.lastMiddleDown=+new Date}
if(start){extendSelection(cm.doc,start)}
setTimeout(function(){return display.input.focus();},20)
e_preventDefault(e)
break
case 3:if(captureRightClick){onContextMenu(cm,e)}
else{delayBlurEvent(cm)}
break}}
var lastClick;var lastDoubleClick;function leftButtonDown(cm,e,start){if(ie){setTimeout(bind(ensureFocus,cm),0)}
else{cm.curOp.focus=activeElt()}
var now=+new Date,type
if(lastDoubleClick&&lastDoubleClick.time>now-400&&cmp(lastDoubleClick.pos,start)==0){type="triple"}else if(lastClick&&lastClick.time>now-400&&cmp(lastClick.pos,start)==0){type="double"
lastDoubleClick={time:now,pos:start}}else{type="single"
lastClick={time:now,pos:start}}
var sel=cm.doc.sel,modifier=mac?e.metaKey:e.ctrlKey,contained
if(cm.options.dragDrop&&dragAndDrop&&!cm.isReadOnly()&&type=="single"&&(contained=sel.contains(start))>-1&&(cmp((contained=sel.ranges[contained]).from(),start)<0||start.xRel>0)&&(cmp(contained.to(),start)>0||start.xRel<0))
{leftButtonStartDrag(cm,e,start,modifier)}
else
{leftButtonSelect(cm,e,start,type,modifier)}}
function leftButtonStartDrag(cm,e,start,modifier){var display=cm.display,startTime=+new Date
var dragEnd=operation(cm,function(e2){if(webkit){display.scroller.draggable=false}
cm.state.draggingText=false
off(document,"mouseup",dragEnd)
off(display.scroller,"drop",dragEnd)
if(Math.abs(e.clientX-e2.clientX)+Math.abs(e.clientY-e2.clientY)<10){e_preventDefault(e2)
if(!modifier&&+new Date-200<startTime)
{extendSelection(cm.doc,start)}
if(webkit||ie&&ie_version==9)
{setTimeout(function(){document.body.focus();display.input.focus()},20)}
else
{display.input.focus()}}})
if(webkit){display.scroller.draggable=true}
cm.state.draggingText=dragEnd
dragEnd.copy=mac?e.altKey:e.ctrlKey
if(display.scroller.dragDrop){display.scroller.dragDrop()}
on(document,"mouseup",dragEnd)
on(display.scroller,"drop",dragEnd)}
function leftButtonSelect(cm,e,start,type,addNew){var display=cm.display,doc=cm.doc
e_preventDefault(e)
var ourRange,ourIndex,startSel=doc.sel,ranges=startSel.ranges
if(addNew&&!e.shiftKey){ourIndex=doc.sel.contains(start)
if(ourIndex>-1)
{ourRange=ranges[ourIndex]}
else
{ourRange=new Range(start,start)}}else{ourRange=doc.sel.primary()
ourIndex=doc.sel.primIndex}
if(chromeOS?e.shiftKey&&e.metaKey:e.altKey){type="rect"
if(!addNew){ourRange=new Range(start,start)}
start=posFromMouse(cm,e,true,true)
ourIndex=-1}else if(type=="double"){var word=cm.findWordAt(start)
if(cm.display.shift||doc.extend)
{ourRange=extendRange(doc,ourRange,word.anchor,word.head)}
else
{ourRange=word}}else if(type=="triple"){var line=new Range(Pos(start.line,0),clipPos(doc,Pos(start.line+1,0)))
if(cm.display.shift||doc.extend)
{ourRange=extendRange(doc,ourRange,line.anchor,line.head)}
else
{ourRange=line}}else{ourRange=extendRange(doc,ourRange,start)}
if(!addNew){ourIndex=0
setSelection(doc,new Selection([ourRange],0),sel_mouse)
startSel=doc.sel}else if(ourIndex==-1){ourIndex=ranges.length
setSelection(doc,normalizeSelection(ranges.concat([ourRange]),ourIndex),{scroll:false,origin:"*mouse"})}else if(ranges.length>1&&ranges[ourIndex].empty()&&type=="single"&&!e.shiftKey){setSelection(doc,normalizeSelection(ranges.slice(0,ourIndex).concat(ranges.slice(ourIndex+1)),0),{scroll:false,origin:"*mouse"})
startSel=doc.sel}else{replaceOneSelection(doc,ourIndex,ourRange,sel_mouse)}
var lastPos=start
function extendTo(pos){if(cmp(lastPos,pos)==0){return}
lastPos=pos
if(type=="rect"){var ranges=[],tabSize=cm.options.tabSize
var startCol=countColumn(getLine(doc,start.line).text,start.ch,tabSize)
var posCol=countColumn(getLine(doc,pos.line).text,pos.ch,tabSize)
var left=Math.min(startCol,posCol),right=Math.max(startCol,posCol)
for(var line=Math.min(start.line,pos.line),end=Math.min(cm.lastLine(),Math.max(start.line,pos.line));line<=end;line++){var text=getLine(doc,line).text,leftPos=findColumn(text,left,tabSize)
if(left==right)
{ranges.push(new Range(Pos(line,leftPos),Pos(line,leftPos)))}
else if(text.length>leftPos)
{ranges.push(new Range(Pos(line,leftPos),Pos(line,findColumn(text,right,tabSize))))}}
if(!ranges.length){ranges.push(new Range(start,start))}
setSelection(doc,normalizeSelection(startSel.ranges.slice(0,ourIndex).concat(ranges),ourIndex),{origin:"*mouse",scroll:false})
cm.scrollIntoView(pos)}else{var oldRange=ourRange
var anchor=oldRange.anchor,head=pos
if(type!="single"){var range
if(type=="double")
{range=cm.findWordAt(pos)}
else
{range=new Range(Pos(pos.line,0),clipPos(doc,Pos(pos.line+1,0)))}
if(cmp(range.anchor,anchor)>0){head=range.head
anchor=minPos(oldRange.from(),range.anchor)}else{head=range.anchor
anchor=maxPos(oldRange.to(),range.head)}}
var ranges$1=startSel.ranges.slice(0)
ranges$1[ourIndex]=new Range(clipPos(doc,anchor),head)
setSelection(doc,normalizeSelection(ranges$1,ourIndex),sel_mouse)}}
var editorSize=display.wrapper.getBoundingClientRect()
var counter=0
function extend(e){var curCount=++counter
var cur=posFromMouse(cm,e,true,type=="rect")
if(!cur){return}
if(cmp(cur,lastPos)!=0){cm.curOp.focus=activeElt()
extendTo(cur)
var visible=visibleLines(display,doc)
if(cur.line>=visible.to||cur.line<visible.from)
{setTimeout(operation(cm,function(){if(counter==curCount){extend(e)}}),150)}}else{var outside=e.clientY<editorSize.top?-20:e.clientY>editorSize.bottom?20:0
if(outside){setTimeout(operation(cm,function(){if(counter!=curCount){return}
display.scroller.scrollTop+=outside
extend(e)}),50)}}}
function done(e){cm.state.selectingText=false
counter=Infinity
e_preventDefault(e)
display.input.focus()
off(document,"mousemove",move)
off(document,"mouseup",up)
doc.history.lastSelOrigin=null}
var move=operation(cm,function(e){if(!e_button(e)){done(e)}
else{extend(e)}})
var up=operation(cm,done)
cm.state.selectingText=up
on(document,"mousemove",move)
on(document,"mouseup",up)}
function gutterEvent(cm,e,type,prevent){var mX,mY
try{mX=e.clientX;mY=e.clientY}
catch(e){return false}
if(mX>=Math.floor(cm.display.gutters.getBoundingClientRect().right)){return false}
if(prevent){e_preventDefault(e)}
var display=cm.display
var lineBox=display.lineDiv.getBoundingClientRect()
if(mY>lineBox.bottom||!hasHandler(cm,type)){return e_defaultPrevented(e)}
mY-=lineBox.top-display.viewOffset
for(var i=0;i<cm.options.gutters.length;++i){var g=display.gutters.childNodes[i]
if(g&&g.getBoundingClientRect().right>=mX){var line=lineAtHeight(cm.doc,mY)
var gutter=cm.options.gutters[i]
signal(cm,type,cm,line,gutter,e)
return e_defaultPrevented(e)}}}
function clickInGutter(cm,e){return gutterEvent(cm,e,"gutterClick",true)}
function onContextMenu(cm,e){if(eventInWidget(cm.display,e)||contextMenuInGutter(cm,e)){return}
if(signalDOMEvent(cm,e,"contextmenu")){return}
cm.display.input.onContextMenu(e)}
function contextMenuInGutter(cm,e){if(!hasHandler(cm,"gutterContextMenu")){return false}
return gutterEvent(cm,e,"gutterContextMenu",false)}
function themeChanged(cm){cm.display.wrapper.className=cm.display.wrapper.className.replace(/\s*cm-s-\S+/g,"")+
cm.options.theme.replace(/(^|\s)\s*/g," cm-s-")
clearCaches(cm)}
var Init={toString:function(){return"CodeMirror.Init"}}
var defaults={}
var optionHandlers={}
function defineOptions(CodeMirror){var optionHandlers=CodeMirror.optionHandlers
function option(name,deflt,handle,notOnInit){CodeMirror.defaults[name]=deflt
if(handle){optionHandlers[name]=notOnInit?function(cm,val,old){if(old!=Init){handle(cm,val,old)}}:handle}}
CodeMirror.defineOption=option
CodeMirror.Init=Init
option("value","",function(cm,val){return cm.setValue(val);},true)
option("mode",null,function(cm,val){cm.doc.modeOption=val
loadMode(cm)},true)
option("indentUnit",2,loadMode,true)
option("indentWithTabs",false)
option("smartIndent",true)
option("tabSize",4,function(cm){resetModeState(cm)
clearCaches(cm)
regChange(cm)},true)
option("lineSeparator",null,function(cm,val){cm.doc.lineSep=val
if(!val){return}
var newBreaks=[],lineNo=cm.doc.first
cm.doc.iter(function(line){for(var pos=0;;){var found=line.text.indexOf(val,pos)
if(found==-1){break}
pos=found+val.length
newBreaks.push(Pos(lineNo,found))}
lineNo++})
for(var i=newBreaks.length-1;i>=0;i--)
{replaceRange(cm.doc,val,newBreaks[i],Pos(newBreaks[i].line,newBreaks[i].ch+val.length))}})
option("specialChars",/[\u0000-\u001f\u007f-\u009f\u00ad\u061c\u200b-\u200f\u2028\u2029\ufeff]/g,function(cm,val,old){cm.state.specialChars=new RegExp(val.source+(val.test("\t")?"":"|\t"),"g")
if(old!=Init){cm.refresh()}})
option("specialCharPlaceholder",defaultSpecialCharPlaceholder,function(cm){return cm.refresh();},true)
option("electricChars",true)
option("inputStyle",mobile?"contenteditable":"textarea",function(){throw new Error("inputStyle can not (yet) be changed in a running editor")},true)
option("spellcheck",false,function(cm,val){return cm.getInputField().spellcheck=val;},true)
option("rtlMoveVisually",!windows)
option("wholeLineUpdateBefore",true)
option("theme","default",function(cm){themeChanged(cm)
guttersChanged(cm)},true)
option("keyMap","default",function(cm,val,old){var next=getKeyMap(val)
var prev=old!=Init&&getKeyMap(old)
if(prev&&prev.detach){prev.detach(cm,next)}
if(next.attach){next.attach(cm,prev||null)}})
option("extraKeys",null)
option("lineWrapping",false,wrappingChanged,true)
option("gutters",[],function(cm){setGuttersForLineNumbers(cm.options)
guttersChanged(cm)},true)
option("fixedGutter",true,function(cm,val){cm.display.gutters.style.left=val?compensateForHScroll(cm.display)+"px":"0"
cm.refresh()},true)
option("coverGutterNextToScrollbar",false,function(cm){return updateScrollbars(cm);},true)
option("scrollbarStyle","native",function(cm){initScrollbars(cm)
updateScrollbars(cm)
cm.display.scrollbars.setScrollTop(cm.doc.scrollTop)
cm.display.scrollbars.setScrollLeft(cm.doc.scrollLeft)},true)
option("lineNumbers",false,function(cm){setGuttersForLineNumbers(cm.options)
guttersChanged(cm)},true)
option("firstLineNumber",1,guttersChanged,true)
option("lineNumberFormatter",function(integer){return integer;},guttersChanged,true)
option("showCursorWhenSelecting",false,updateSelection,true)
option("resetSelectionOnContextMenu",true)
option("lineWiseCopyCut",true)
option("readOnly",false,function(cm,val){if(val=="nocursor"){onBlur(cm)
cm.display.input.blur()
cm.display.disabled=true}else{cm.display.disabled=false}
cm.display.input.readOnlyChanged(val)})
option("disableInput",false,function(cm,val){if(!val){cm.display.input.reset()}},true)
option("dragDrop",true,dragDropChanged)
option("allowDropFileTypes",null)
option("cursorBlinkRate",530)
option("cursorScrollMargin",0)
option("cursorHeight",1,updateSelection,true)
option("singleCursorHeightPerLine",true,updateSelection,true)
option("workTime",100)
option("workDelay",100)
option("flattenSpans",true,resetModeState,true)
option("addModeClass",false,resetModeState,true)
option("pollInterval",100)
option("undoDepth",200,function(cm,val){return cm.doc.history.undoDepth=val;})
option("historyEventDelay",1250)
option("viewportMargin",10,function(cm){return cm.refresh();},true)
option("maxHighlightLength",10000,resetModeState,true)
option("moveInputWithCursor",true,function(cm,val){if(!val){cm.display.input.resetPosition()}})
option("tabindex",null,function(cm,val){return cm.display.input.getField().tabIndex=val||"";})
option("autofocus",null)
option("direction","ltr",function(cm,val){return cm.doc.setDirection(val);},true)}
function guttersChanged(cm){updateGutters(cm)
regChange(cm)
alignHorizontally(cm)}
function dragDropChanged(cm,value,old){var wasOn=old&&old!=Init
if(!value!=!wasOn){var funcs=cm.display.dragFunctions
var toggle=value?on:off
toggle(cm.display.scroller,"dragstart",funcs.start)
toggle(cm.display.scroller,"dragenter",funcs.enter)
toggle(cm.display.scroller,"dragover",funcs.over)
toggle(cm.display.scroller,"dragleave",funcs.leave)
toggle(cm.display.scroller,"drop",funcs.drop)}}
function wrappingChanged(cm){if(cm.options.lineWrapping){addClass(cm.display.wrapper,"CodeMirror-wrap")
cm.display.sizer.style.minWidth=""
cm.display.sizerWidth=null}else{rmClass(cm.display.wrapper,"CodeMirror-wrap")
findMaxLine(cm)}
estimateLineHeights(cm)
regChange(cm)
clearCaches(cm)
setTimeout(function(){return updateScrollbars(cm);},100)}
function CodeMirror(place,options){var this$1=this;if(!(this instanceof CodeMirror)){return new CodeMirror(place,options)}
this.options=options=options?copyObj(options):{}
copyObj(defaults,options,false)
setGuttersForLineNumbers(options)
var doc=options.value
if(typeof doc=="string"){doc=new Doc(doc,options.mode,null,options.lineSeparator,options.direction)}
this.doc=doc
var input=new CodeMirror.inputStyles[options.inputStyle](this)
var display=this.display=new Display(place,doc,input)
display.wrapper.CodeMirror=this
updateGutters(this)
themeChanged(this)
if(options.lineWrapping)
{this.display.wrapper.className+=" CodeMirror-wrap"}
initScrollbars(this)
this.state={keyMaps:[],overlays:[],modeGen:0,overwrite:false,delayingBlurEvent:false,focused:false,suppressEdits:false,pasteIncoming:false,cutIncoming:false,selectingText:false,draggingText:false,highlight:new Delayed(),keySeq:null,specialChars:null}
if(options.autofocus&&!mobile){display.input.focus()}
if(ie&&ie_version<11){setTimeout(function(){return this$1.display.input.reset(true);},20)}
registerEventHandlers(this)
ensureGlobalHandlers()
startOperation(this)
this.curOp.forceUpdate=true
attachDoc(this,doc)
if((options.autofocus&&!mobile)||this.hasFocus())
{setTimeout(bind(onFocus,this),20)}
else
{onBlur(this)}
for(var opt in optionHandlers){if(optionHandlers.hasOwnProperty(opt))
{optionHandlers[opt](this$1,options[opt],Init)}}
maybeUpdateLineNumberWidth(this)
if(options.finishInit){options.finishInit(this)}
for(var i=0;i<initHooks.length;++i){initHooks[i](this$1)}
endOperation(this)
if(webkit&&options.lineWrapping&&getComputedStyle(display.lineDiv).textRendering=="optimizelegibility")
{display.lineDiv.style.textRendering="auto"}}
CodeMirror.defaults=defaults
CodeMirror.optionHandlers=optionHandlers
function registerEventHandlers(cm){var d=cm.display
on(d.scroller,"mousedown",operation(cm,onMouseDown))
if(ie&&ie_version<11)
{on(d.scroller,"dblclick",operation(cm,function(e){if(signalDOMEvent(cm,e)){return}
var pos=posFromMouse(cm,e)
if(!pos||clickInGutter(cm,e)||eventInWidget(cm.display,e)){return}
e_preventDefault(e)
var word=cm.findWordAt(pos)
extendSelection(cm.doc,word.anchor,word.head)}))}
else
{on(d.scroller,"dblclick",function(e){return signalDOMEvent(cm,e)||e_preventDefault(e);})}
if(!captureRightClick){on(d.scroller,"contextmenu",function(e){return onContextMenu(cm,e);})}
var touchFinished,prevTouch={end:0}
function finishTouch(){if(d.activeTouch){touchFinished=setTimeout(function(){return d.activeTouch=null;},1000)
prevTouch=d.activeTouch
prevTouch.end=+new Date}}
function isMouseLikeTouchEvent(e){if(e.touches.length!=1){return false}
var touch=e.touches[0]
return touch.radiusX<=1&&touch.radiusY<=1}
function farAway(touch,other){if(other.left==null){return true}
var dx=other.left-touch.left,dy=other.top-touch.top
return dx*dx+dy*dy>20*20}
on(d.scroller,"touchstart",function(e){if(!signalDOMEvent(cm,e)&&!isMouseLikeTouchEvent(e)){d.input.ensurePolled()
clearTimeout(touchFinished)
var now=+new Date
d.activeTouch={start:now,moved:false,prev:now-prevTouch.end<=300?prevTouch:null}
if(e.touches.length==1){d.activeTouch.left=e.touches[0].pageX
d.activeTouch.top=e.touches[0].pageY}}})
on(d.scroller,"touchmove",function(){if(d.activeTouch){d.activeTouch.moved=true}})
on(d.scroller,"touchend",function(e){var touch=d.activeTouch
if(touch&&!eventInWidget(d,e)&&touch.left!=null&&!touch.moved&&new Date-touch.start<300){var pos=cm.coordsChar(d.activeTouch,"page"),range
if(!touch.prev||farAway(touch,touch.prev))
{range=new Range(pos,pos)}
else if(!touch.prev.prev||farAway(touch,touch.prev.prev))
{range=cm.findWordAt(pos)}
else
{range=new Range(Pos(pos.line,0),clipPos(cm.doc,Pos(pos.line+1,0)))}
cm.setSelection(range.anchor,range.head)
cm.focus()
e_preventDefault(e)}
finishTouch()})
on(d.scroller,"touchcancel",finishTouch)
on(d.scroller,"scroll",function(){if(d.scroller.clientHeight){setScrollTop(cm,d.scroller.scrollTop)
setScrollLeft(cm,d.scroller.scrollLeft,true)
signal(cm,"scroll",cm)}})
on(d.scroller,"mousewheel",function(e){return onScrollWheel(cm,e);})
on(d.scroller,"DOMMouseScroll",function(e){return onScrollWheel(cm,e);})
on(d.wrapper,"scroll",function(){return d.wrapper.scrollTop=d.wrapper.scrollLeft=0;})
d.dragFunctions={enter:function(e){if(!signalDOMEvent(cm,e)){e_stop(e)}},over:function(e){if(!signalDOMEvent(cm,e)){onDragOver(cm,e);e_stop(e)}},start:function(e){return onDragStart(cm,e);},drop:operation(cm,onDrop),leave:function(e){if(!signalDOMEvent(cm,e)){clearDragCursor(cm)}}}
var inp=d.input.getField()
on(inp,"keyup",function(e){return onKeyUp.call(cm,e);})
on(inp,"keydown",operation(cm,onKeyDown))
on(inp,"keypress",operation(cm,onKeyPress))
on(inp,"focus",function(e){return onFocus(cm,e);})
on(inp,"blur",function(e){return onBlur(cm,e);})}
var initHooks=[]
CodeMirror.defineInitHook=function(f){return initHooks.push(f);}
function indentLine(cm,n,how,aggressive){var doc=cm.doc,state
if(how==null){how="add"}
if(how=="smart"){if(!doc.mode.indent){how="prev"}
else{state=getStateBefore(cm,n)}}
var tabSize=cm.options.tabSize
var line=getLine(doc,n),curSpace=countColumn(line.text,null,tabSize)
if(line.stateAfter){line.stateAfter=null}
var curSpaceString=line.text.match(/^\s*/)[0],indentation
if(!aggressive&&!/\S/.test(line.text)){indentation=0
how="not"}else if(how=="smart"){indentation=doc.mode.indent(state,line.text.slice(curSpaceString.length),line.text)
if(indentation==Pass||indentation>150){if(!aggressive){return}
how="prev"}}
if(how=="prev"){if(n>doc.first){indentation=countColumn(getLine(doc,n-1).text,null,tabSize)}
else{indentation=0}}else if(how=="add"){indentation=curSpace+cm.options.indentUnit}else if(how=="subtract"){indentation=curSpace-cm.options.indentUnit}else if(typeof how=="number"){indentation=curSpace+how}
indentation=Math.max(0,indentation)
var indentString="",pos=0
if(cm.options.indentWithTabs)
{for(var i=Math.floor(indentation/tabSize);i;--i){pos+=tabSize;indentString+="\t"}}
if(pos<indentation){indentString+=spaceStr(indentation-pos)}
if(indentString!=curSpaceString){replaceRange(doc,indentString,Pos(n,0),Pos(n,curSpaceString.length),"+input")
line.stateAfter=null
return true}else{for(var i$1=0;i$1<doc.sel.ranges.length;i$1++){var range=doc.sel.ranges[i$1]
if(range.head.line==n&&range.head.ch<curSpaceString.length){var pos$1=Pos(n,curSpaceString.length)
replaceOneSelection(doc,i$1,new Range(pos$1,pos$1))
break}}}}
var lastCopied=null
function setLastCopied(newLastCopied){lastCopied=newLastCopied}
function applyTextInput(cm,inserted,deleted,sel,origin){var doc=cm.doc
cm.display.shift=false
if(!sel){sel=doc.sel}
var paste=cm.state.pasteIncoming||origin=="paste"
var textLines=splitLinesAuto(inserted),multiPaste=null
if(paste&&sel.ranges.length>1){if(lastCopied&&lastCopied.text.join("\n")==inserted){if(sel.ranges.length%lastCopied.text.length==0){multiPaste=[]
for(var i=0;i<lastCopied.text.length;i++)
{multiPaste.push(doc.splitLines(lastCopied.text[i]))}}}else if(textLines.length==sel.ranges.length){multiPaste=map(textLines,function(l){return[l];})}}
var updateInput
for(var i$1=sel.ranges.length-1;i$1>=0;i$1--){var range=sel.ranges[i$1]
var from=range.from(),to=range.to()
if(range.empty()){if(deleted&&deleted>0)
{from=Pos(from.line,from.ch-deleted)}
else if(cm.state.overwrite&&!paste)
{to=Pos(to.line,Math.min(getLine(doc,to.line).text.length,to.ch+lst(textLines).length))}
else if(lastCopied&&lastCopied.lineWise&&lastCopied.text.join("\n")==inserted)
{from=to=Pos(from.line,0)}}
updateInput=cm.curOp.updateInput
var changeEvent={from:from,to:to,text:multiPaste?multiPaste[i$1%multiPaste.length]:textLines,origin:origin||(paste?"paste":cm.state.cutIncoming?"cut":"+input")}
makeChange(cm.doc,changeEvent)
signalLater(cm,"inputRead",cm,changeEvent)}
if(inserted&&!paste)
{triggerElectric(cm,inserted)}
ensureCursorVisible(cm)
cm.curOp.updateInput=updateInput
cm.curOp.typing=true
cm.state.pasteIncoming=cm.state.cutIncoming=false}
function handlePaste(e,cm){var pasted=e.clipboardData&&e.clipboardData.getData("Text")
if(pasted){e.preventDefault()
if(!cm.isReadOnly()&&!cm.options.disableInput)
{runInOp(cm,function(){return applyTextInput(cm,pasted,0,null,"paste");})}
return true}}
function triggerElectric(cm,inserted){if(!cm.options.electricChars||!cm.options.smartIndent){return}
var sel=cm.doc.sel
for(var i=sel.ranges.length-1;i>=0;i--){var range=sel.ranges[i]
if(range.head.ch>100||(i&&sel.ranges[i-1].head.line==range.head.line)){continue}
var mode=cm.getModeAt(range.head)
var indented=false
if(mode.electricChars){for(var j=0;j<mode.electricChars.length;j++)
{if(inserted.indexOf(mode.electricChars.charAt(j))>-1){indented=indentLine(cm,range.head.line,"smart")
break}}}else if(mode.electricInput){if(mode.electricInput.test(getLine(cm.doc,range.head.line).text.slice(0,range.head.ch)))
{indented=indentLine(cm,range.head.line,"smart")}}
if(indented){signalLater(cm,"electricInput",cm,range.head.line)}}}
function copyableRanges(cm){var text=[],ranges=[]
for(var i=0;i<cm.doc.sel.ranges.length;i++){var line=cm.doc.sel.ranges[i].head.line
var lineRange={anchor:Pos(line,0),head:Pos(line+1,0)}
ranges.push(lineRange)
text.push(cm.getRange(lineRange.anchor,lineRange.head))}
return{text:text,ranges:ranges}}
function disableBrowserMagic(field,spellcheck){field.setAttribute("autocorrect","off")
field.setAttribute("autocapitalize","off")
field.setAttribute("spellcheck",!!spellcheck)}
function hiddenTextarea(){var te=elt("textarea",null,null,"position: absolute; bottom: -1em; padding: 0; width: 1px; height: 1em; outline: none")
var div=elt("div",[te],null,"overflow: hidden; position: relative; width: 3px; height: 0px;")
if(webkit){te.style.width="1000px"}
else{te.setAttribute("wrap","off")}
if(ios){te.style.border="1px solid black"}
disableBrowserMagic(te)
return div}
function addEditorMethods(CodeMirror){var optionHandlers=CodeMirror.optionHandlers
var helpers=CodeMirror.helpers={}
CodeMirror.prototype={constructor:CodeMirror,focus:function(){window.focus();this.display.input.focus()},setOption:function(option,value){var options=this.options,old=options[option]
if(options[option]==value&&option!="mode"){return}
options[option]=value
if(optionHandlers.hasOwnProperty(option))
{operation(this,optionHandlers[option])(this,value,old)}
signal(this,"optionChange",this,option)},getOption:function(option){return this.options[option]},getDoc:function(){return this.doc},addKeyMap:function(map,bottom){this.state.keyMaps[bottom?"push":"unshift"](getKeyMap(map))},removeKeyMap:function(map){var maps=this.state.keyMaps
for(var i=0;i<maps.length;++i)
{if(maps[i]==map||maps[i].name==map){maps.splice(i,1)
return true}}},addOverlay:methodOp(function(spec,options){var mode=spec.token?spec:CodeMirror.getMode(this.options,spec)
if(mode.startState){throw new Error("Overlays may not be stateful.")}
insertSorted(this.state.overlays,{mode:mode,modeSpec:spec,opaque:options&&options.opaque,priority:(options&&options.priority)||0},function(overlay){return overlay.priority;})
this.state.modeGen++
regChange(this)}),removeOverlay:methodOp(function(spec){var this$1=this;var overlays=this.state.overlays
for(var i=0;i<overlays.length;++i){var cur=overlays[i].modeSpec
if(cur==spec||typeof spec=="string"&&cur.name==spec){overlays.splice(i,1)
this$1.state.modeGen++
regChange(this$1)
return}}}),indentLine:methodOp(function(n,dir,aggressive){if(typeof dir!="string"&&typeof dir!="number"){if(dir==null){dir=this.options.smartIndent?"smart":"prev"}
else{dir=dir?"add":"subtract"}}
if(isLine(this.doc,n)){indentLine(this,n,dir,aggressive)}}),indentSelection:methodOp(function(how){var this$1=this;var ranges=this.doc.sel.ranges,end=-1
for(var i=0;i<ranges.length;i++){var range=ranges[i]
if(!range.empty()){var from=range.from(),to=range.to()
var start=Math.max(end,from.line)
end=Math.min(this$1.lastLine(),to.line-(to.ch?0:1))+1
for(var j=start;j<end;++j)
{indentLine(this$1,j,how)}
var newRanges=this$1.doc.sel.ranges
if(from.ch==0&&ranges.length==newRanges.length&&newRanges[i].from().ch>0)
{replaceOneSelection(this$1.doc,i,new Range(from,newRanges[i].to()),sel_dontScroll)}}else if(range.head.line>end){indentLine(this$1,range.head.line,how,true)
end=range.head.line
if(i==this$1.doc.sel.primIndex){ensureCursorVisible(this$1)}}}}),getTokenAt:function(pos,precise){return takeToken(this,pos,precise)},getLineTokens:function(line,precise){return takeToken(this,Pos(line),precise,true)},getTokenTypeAt:function(pos){pos=clipPos(this.doc,pos)
var styles=getLineStyles(this,getLine(this.doc,pos.line))
var before=0,after=(styles.length-1)/2,ch=pos.ch
var type
if(ch==0){type=styles[2]}
else{for(;;){var mid=(before+after)>>1
if((mid?styles[mid*2-1]:0)>=ch){after=mid}
else if(styles[mid*2+1]<ch){before=mid+1}
else{type=styles[mid*2+2];break}}}
var cut=type?type.indexOf("overlay "):-1
return cut<0?type:cut==0?null:type.slice(0,cut-1)},getModeAt:function(pos){var mode=this.doc.mode
if(!mode.innerMode){return mode}
return CodeMirror.innerMode(mode,this.getTokenAt(pos).state).mode},getHelper:function(pos,type){return this.getHelpers(pos,type)[0]},getHelpers:function(pos,type){var this$1=this;var found=[]
if(!helpers.hasOwnProperty(type)){return found}
var help=helpers[type],mode=this.getModeAt(pos)
if(typeof mode[type]=="string"){if(help[mode[type]]){found.push(help[mode[type]])}}else if(mode[type]){for(var i=0;i<mode[type].length;i++){var val=help[mode[type][i]]
if(val){found.push(val)}}}else if(mode.helperType&&help[mode.helperType]){found.push(help[mode.helperType])}else if(help[mode.name]){found.push(help[mode.name])}
for(var i$1=0;i$1<help._global.length;i$1++){var cur=help._global[i$1]
if(cur.pred(mode,this$1)&&indexOf(found,cur.val)==-1)
{found.push(cur.val)}}
return found},getStateAfter:function(line,precise){var doc=this.doc
line=clipLine(doc,line==null?doc.first+doc.size-1:line)
return getStateBefore(this,line+1,precise)},cursorCoords:function(start,mode){var pos,range=this.doc.sel.primary()
if(start==null){pos=range.head}
else if(typeof start=="object"){pos=clipPos(this.doc,start)}
else{pos=start?range.from():range.to()}
return cursorCoords(this,pos,mode||"page")},charCoords:function(pos,mode){return charCoords(this,clipPos(this.doc,pos),mode||"page")},coordsChar:function(coords,mode){coords=fromCoordSystem(this,coords,mode||"page")
return coordsChar(this,coords.left,coords.top)},lineAtHeight:function(height,mode){height=fromCoordSystem(this,{top:height,left:0},mode||"page").top
return lineAtHeight(this.doc,height+this.display.viewOffset)},heightAtLine:function(line,mode,includeWidgets){var end=false,lineObj
if(typeof line=="number"){var last=this.doc.first+this.doc.size-1
if(line<this.doc.first){line=this.doc.first}
else if(line>last){line=last;end=true}
lineObj=getLine(this.doc,line)}else{lineObj=line}
return intoCoordSystem(this,lineObj,{top:0,left:0},mode||"page",includeWidgets||end).top+
(end?this.doc.height-heightAtLine(lineObj):0)},defaultTextHeight:function(){return textHeight(this.display)},defaultCharWidth:function(){return charWidth(this.display)},getViewport:function(){return{from:this.display.viewFrom,to:this.display.viewTo}},addWidget:function(pos,node,scroll,vert,horiz){var display=this.display
pos=cursorCoords(this,clipPos(this.doc,pos))
var top=pos.bottom,left=pos.left
node.style.position="absolute"
node.setAttribute("cm-ignore-events","true")
this.display.input.setUneditable(node)
display.sizer.appendChild(node)
if(vert=="over"){top=pos.top}else if(vert=="above"||vert=="near"){var vspace=Math.max(display.wrapper.clientHeight,this.doc.height),hspace=Math.max(display.sizer.clientWidth,display.lineSpace.clientWidth)
if((vert=='above'||pos.bottom+node.offsetHeight>vspace)&&pos.top>node.offsetHeight)
{top=pos.top-node.offsetHeight}
else if(pos.bottom+node.offsetHeight<=vspace)
{top=pos.bottom}
if(left+node.offsetWidth>hspace)
{left=hspace-node.offsetWidth}}
node.style.top=top+"px"
node.style.left=node.style.right=""
if(horiz=="right"){left=display.sizer.clientWidth-node.offsetWidth
node.style.right="0px"}else{if(horiz=="left"){left=0}
else if(horiz=="middle"){left=(display.sizer.clientWidth-node.offsetWidth)/2}
node.style.left=left+"px"}
if(scroll)
{scrollIntoView(this,{left:left,top:top,right:left+node.offsetWidth,bottom:top+node.offsetHeight})}},triggerOnKeyDown:methodOp(onKeyDown),triggerOnKeyPress:methodOp(onKeyPress),triggerOnKeyUp:onKeyUp,execCommand:function(cmd){if(commands.hasOwnProperty(cmd))
{return commands[cmd].call(null,this)}},triggerElectric:methodOp(function(text){triggerElectric(this,text)}),findPosH:function(from,amount,unit,visually){var this$1=this;var dir=1
if(amount<0){dir=-1;amount=-amount}
var cur=clipPos(this.doc,from)
for(var i=0;i<amount;++i){cur=findPosH(this$1.doc,cur,dir,unit,visually)
if(cur.hitSide){break}}
return cur},moveH:methodOp(function(dir,unit){var this$1=this;this.extendSelectionsBy(function(range){if(this$1.display.shift||this$1.doc.extend||range.empty())
{return findPosH(this$1.doc,range.head,dir,unit,this$1.options.rtlMoveVisually)}
else
{return dir<0?range.from():range.to()}},sel_move)}),deleteH:methodOp(function(dir,unit){var sel=this.doc.sel,doc=this.doc
if(sel.somethingSelected())
{doc.replaceSelection("",null,"+delete")}
else
{deleteNearSelection(this,function(range){var other=findPosH(doc,range.head,dir,unit,false)
return dir<0?{from:other,to:range.head}:{from:range.head,to:other}})}}),findPosV:function(from,amount,unit,goalColumn){var this$1=this;var dir=1,x=goalColumn
if(amount<0){dir=-1;amount=-amount}
var cur=clipPos(this.doc,from)
for(var i=0;i<amount;++i){var coords=cursorCoords(this$1,cur,"div")
if(x==null){x=coords.left}
else{coords.left=x}
cur=findPosV(this$1,coords,dir,unit)
if(cur.hitSide){break}}
return cur},moveV:methodOp(function(dir,unit){var this$1=this;var doc=this.doc,goals=[]
var collapse=!this.display.shift&&!doc.extend&&doc.sel.somethingSelected()
doc.extendSelectionsBy(function(range){if(collapse)
{return dir<0?range.from():range.to()}
var headPos=cursorCoords(this$1,range.head,"div")
if(range.goalColumn!=null){headPos.left=range.goalColumn}
goals.push(headPos.left)
var pos=findPosV(this$1,headPos,dir,unit)
if(unit=="page"&&range==doc.sel.primary())
{addToScrollPos(this$1,null,charCoords(this$1,pos,"div").top-headPos.top)}
return pos},sel_move)
if(goals.length){for(var i=0;i<doc.sel.ranges.length;i++)
{doc.sel.ranges[i].goalColumn=goals[i]}}}),findWordAt:function(pos){var doc=this.doc,line=getLine(doc,pos.line).text
var start=pos.ch,end=pos.ch
if(line){var helper=this.getHelper(pos,"wordChars")
if((pos.sticky=="before"||end==line.length)&&start){--start;}else{++end}
var startChar=line.charAt(start)
var check=isWordChar(startChar,helper)?function(ch){return isWordChar(ch,helper);}:/\s/.test(startChar)?function(ch){return/\s/.test(ch);}:function(ch){return(!/\s/.test(ch)&&!isWordChar(ch));}
while(start>0&&check(line.charAt(start-1))){--start}
while(end<line.length&&check(line.charAt(end))){++end}}
return new Range(Pos(pos.line,start),Pos(pos.line,end))},toggleOverwrite:function(value){if(value!=null&&value==this.state.overwrite){return}
if(this.state.overwrite=!this.state.overwrite)
{addClass(this.display.cursorDiv,"CodeMirror-overwrite")}
else
{rmClass(this.display.cursorDiv,"CodeMirror-overwrite")}
signal(this,"overwriteToggle",this,this.state.overwrite)},hasFocus:function(){return this.display.input.getField()==activeElt()},isReadOnly:function(){return!!(this.options.readOnly||this.doc.cantEdit)},scrollTo:methodOp(function(x,y){if(x!=null||y!=null){resolveScrollToPos(this)}
if(x!=null){this.curOp.scrollLeft=x}
if(y!=null){this.curOp.scrollTop=y}}),getScrollInfo:function(){var scroller=this.display.scroller
return{left:scroller.scrollLeft,top:scroller.scrollTop,height:scroller.scrollHeight-scrollGap(this)-this.display.barHeight,width:scroller.scrollWidth-scrollGap(this)-this.display.barWidth,clientHeight:displayHeight(this),clientWidth:displayWidth(this)}},scrollIntoView:methodOp(function(range,margin){if(range==null){range={from:this.doc.sel.primary().head,to:null}
if(margin==null){margin=this.options.cursorScrollMargin}}else if(typeof range=="number"){range={from:Pos(range,0),to:null}}else if(range.from==null){range={from:range,to:null}}
if(!range.to){range.to=range.from}
range.margin=margin||0
if(range.from.line!=null){resolveScrollToPos(this)
this.curOp.scrollToPos=range}else{var sPos=calculateScrollPos(this,{left:Math.min(range.from.left,range.to.left),top:Math.min(range.from.top,range.to.top)-range.margin,right:Math.max(range.from.right,range.to.right),bottom:Math.max(range.from.bottom,range.to.bottom)+range.margin})
this.scrollTo(sPos.scrollLeft,sPos.scrollTop)}}),setSize:methodOp(function(width,height){var this$1=this;var interpret=function(val){return typeof val=="number"||/^\d+$/.test(String(val))?val+"px":val;}
if(width!=null){this.display.wrapper.style.width=interpret(width)}
if(height!=null){this.display.wrapper.style.height=interpret(height)}
if(this.options.lineWrapping){clearLineMeasurementCache(this)}
var lineNo=this.display.viewFrom
this.doc.iter(lineNo,this.display.viewTo,function(line){if(line.widgets){for(var i=0;i<line.widgets.length;i++)
{if(line.widgets[i].noHScroll){regLineChange(this$1,lineNo,"widget");break}}}
++lineNo})
this.curOp.forceUpdate=true
signal(this,"refresh",this)}),operation:function(f){return runInOp(this,f)},refresh:methodOp(function(){var oldHeight=this.display.cachedTextHeight
regChange(this)
this.curOp.forceUpdate=true
clearCaches(this)
this.scrollTo(this.doc.scrollLeft,this.doc.scrollTop)
updateGutterSpace(this)
if(oldHeight==null||Math.abs(oldHeight-textHeight(this.display))>.5)
{estimateLineHeights(this)}
signal(this,"refresh",this)}),swapDoc:methodOp(function(doc){var old=this.doc
old.cm=null
attachDoc(this,doc)
clearCaches(this)
this.display.input.reset()
this.scrollTo(doc.scrollLeft,doc.scrollTop)
this.curOp.forceScroll=true
signalLater(this,"swapDoc",this,old)
return old}),getInputField:function(){return this.display.input.getField()},getWrapperElement:function(){return this.display.wrapper},getScrollerElement:function(){return this.display.scroller},getGutterElement:function(){return this.display.gutters}}
eventMixin(CodeMirror)
CodeMirror.registerHelper=function(type,name,value){if(!helpers.hasOwnProperty(type)){helpers[type]=CodeMirror[type]={_global:[]}}
helpers[type][name]=value}
CodeMirror.registerGlobalHelper=function(type,name,predicate,value){CodeMirror.registerHelper(type,name,value)
helpers[type]._global.push({pred:predicate,val:value})}}
function findPosH(doc,pos,dir,unit,visually){var oldPos=pos
var origDir=dir
var lineObj=getLine(doc,pos.line)
function findNextLine(){var l=pos.line+dir
if(l<doc.first||l>=doc.first+doc.size){return false}
pos=new Pos(l,pos.ch,pos.sticky)
return lineObj=getLine(doc,l)}
function moveOnce(boundToLine){var next
if(visually){next=moveVisually(doc.cm,lineObj,pos,dir)}else{next=moveLogically(lineObj,pos,dir)}
if(next==null){if(!boundToLine&&findNextLine())
{pos=endOfLine(visually,doc.cm,lineObj,pos.line,dir)}
else
{return false}}else{pos=next}
return true}
if(unit=="char"){moveOnce()}else if(unit=="column"){moveOnce(true)}else if(unit=="word"||unit=="group"){var sawType=null,group=unit=="group"
var helper=doc.cm&&doc.cm.getHelper(pos,"wordChars")
for(var first=true;;first=false){if(dir<0&&!moveOnce(!first)){break}
var cur=lineObj.text.charAt(pos.ch)||"\n"
var type=isWordChar(cur,helper)?"w":group&&cur=="\n"?"n":!group||/\s/.test(cur)?null:"p"
if(group&&!first&&!type){type="s"}
if(sawType&&sawType!=type){if(dir<0){dir=1;moveOnce();pos.sticky="after"}
break}
if(type){sawType=type}
if(dir>0&&!moveOnce(!first)){break}}}
var result=skipAtomic(doc,pos,oldPos,origDir,true)
if(equalCursorPos(oldPos,result)){result.hitSide=true}
return result}
function findPosV(cm,pos,dir,unit){var doc=cm.doc,x=pos.left,y
if(unit=="page"){var pageSize=Math.min(cm.display.wrapper.clientHeight,window.innerHeight||document.documentElement.clientHeight)
var moveAmount=Math.max(pageSize-.5*textHeight(cm.display),3)
y=(dir>0?pos.bottom:pos.top)+dir*moveAmount}else if(unit=="line"){y=dir>0?pos.bottom+3:pos.top-3}
var target
for(;;){target=coordsChar(cm,x,y)
if(!target.outside){break}
if(dir<0?y<=0:y>=doc.height){target.hitSide=true;break}
y+=dir*5}
return target}
var ContentEditableInput=function ContentEditableInput(cm){this.cm=cm
this.lastAnchorNode=this.lastAnchorOffset=this.lastFocusNode=this.lastFocusOffset=null
this.polling=new Delayed()
this.composing=null
this.gracePeriod=false
this.readDOMTimeout=null};ContentEditableInput.prototype.init=function init(display){var this$1=this;var input=this,cm=input.cm
var div=input.div=display.lineDiv
disableBrowserMagic(div,cm.options.spellcheck)
on(div,"paste",function(e){if(signalDOMEvent(cm,e)||handlePaste(e,cm)){return}
if(ie_version<=11){setTimeout(operation(cm,function(){return this$1.updateFromDOM();}),20)}})
on(div,"compositionstart",function(e){this$1.composing={data:e.data,done:false}})
on(div,"compositionupdate",function(e){if(!this$1.composing){this$1.composing={data:e.data,done:false}}})
on(div,"compositionend",function(e){if(this$1.composing){if(e.data!=this$1.composing.data){this$1.readFromDOMSoon()}
this$1.composing.done=true}})
on(div,"touchstart",function(){return input.forceCompositionEnd();})
on(div,"input",function(){if(!this$1.composing){this$1.readFromDOMSoon()}})
function onCopyCut(e){if(signalDOMEvent(cm,e)){return}
if(cm.somethingSelected()){setLastCopied({lineWise:false,text:cm.getSelections()})
if(e.type=="cut"){cm.replaceSelection("",null,"cut")}}else if(!cm.options.lineWiseCopyCut){return}else{var ranges=copyableRanges(cm)
setLastCopied({lineWise:true,text:ranges.text})
if(e.type=="cut"){cm.operation(function(){cm.setSelections(ranges.ranges,0,sel_dontScroll)
cm.replaceSelection("",null,"cut")})}}
if(e.clipboardData){e.clipboardData.clearData()
var content=lastCopied.text.join("\n")
e.clipboardData.setData("Text",content)
if(e.clipboardData.getData("Text")==content){e.preventDefault()
return}}
var kludge=hiddenTextarea(),te=kludge.firstChild
cm.display.lineSpace.insertBefore(kludge,cm.display.lineSpace.firstChild)
te.value=lastCopied.text.join("\n")
var hadFocus=document.activeElement
selectInput(te)
setTimeout(function(){cm.display.lineSpace.removeChild(kludge)
hadFocus.focus()
if(hadFocus==div){input.showPrimarySelection()}},50)}
on(div,"copy",onCopyCut)
on(div,"cut",onCopyCut)};ContentEditableInput.prototype.prepareSelection=function prepareSelection$1(){var result=prepareSelection(this.cm,false)
result.focus=this.cm.state.focused
return result};ContentEditableInput.prototype.showSelection=function showSelection(info,takeFocus){if(!info||!this.cm.display.view.length){return}
if(info.focus||takeFocus){this.showPrimarySelection()}
this.showMultipleSelections(info)};ContentEditableInput.prototype.showPrimarySelection=function showPrimarySelection(){var sel=window.getSelection(),prim=this.cm.doc.sel.primary()
var curAnchor=domToPos(this.cm,sel.anchorNode,sel.anchorOffset)
var curFocus=domToPos(this.cm,sel.focusNode,sel.focusOffset)
if(curAnchor&&!curAnchor.bad&&curFocus&&!curFocus.bad&&cmp(minPos(curAnchor,curFocus),prim.from())==0&&cmp(maxPos(curAnchor,curFocus),prim.to())==0)
{return}
var start=posToDOM(this.cm,prim.from())
var end=posToDOM(this.cm,prim.to())
if(!start&&!end){sel.removeAllRanges()
return}
var view=this.cm.display.view
var old=sel.rangeCount&&sel.getRangeAt(0)
if(!start){start={node:view[0].measure.map[2],offset:0}}else if(!end){var measure=view[view.length-1].measure
var map=measure.maps?measure.maps[measure.maps.length-1]:measure.map
end={node:map[map.length-1],offset:map[map.length-2]-map[map.length-3]}}
var rng
try{rng=range(start.node,start.offset,end.offset,end.node)}
catch(e){}
if(rng){if(!gecko&&this.cm.state.focused){sel.collapse(start.node,start.offset)
if(!rng.collapsed){sel.removeAllRanges()
sel.addRange(rng)}}else{sel.removeAllRanges()
sel.addRange(rng)}
if(old&&sel.anchorNode==null){sel.addRange(old)}
else if(gecko){this.startGracePeriod()}}
this.rememberSelection()};ContentEditableInput.prototype.startGracePeriod=function startGracePeriod(){var this$1=this;clearTimeout(this.gracePeriod)
this.gracePeriod=setTimeout(function(){this$1.gracePeriod=false
if(this$1.selectionChanged())
{this$1.cm.operation(function(){return this$1.cm.curOp.selectionChanged=true;})}},20)};ContentEditableInput.prototype.showMultipleSelections=function showMultipleSelections(info){removeChildrenAndAdd(this.cm.display.cursorDiv,info.cursors)
removeChildrenAndAdd(this.cm.display.selectionDiv,info.selection)};ContentEditableInput.prototype.rememberSelection=function rememberSelection(){var sel=window.getSelection()
this.lastAnchorNode=sel.anchorNode;this.lastAnchorOffset=sel.anchorOffset
this.lastFocusNode=sel.focusNode;this.lastFocusOffset=sel.focusOffset};ContentEditableInput.prototype.selectionInEditor=function selectionInEditor(){var sel=window.getSelection()
if(!sel.rangeCount){return false}
var node=sel.getRangeAt(0).commonAncestorContainer
return contains(this.div,node)};ContentEditableInput.prototype.focus=function focus(){if(this.cm.options.readOnly!="nocursor"){if(!this.selectionInEditor())
{this.showSelection(this.prepareSelection(),true)}
this.div.focus()}};ContentEditableInput.prototype.blur=function blur(){this.div.blur()};ContentEditableInput.prototype.getField=function getField(){return this.div};ContentEditableInput.prototype.supportsTouch=function supportsTouch(){return true};ContentEditableInput.prototype.receivedFocus=function receivedFocus(){var input=this
if(this.selectionInEditor())
{this.pollSelection()}
else
{runInOp(this.cm,function(){return input.cm.curOp.selectionChanged=true;})}
function poll(){if(input.cm.state.focused){input.pollSelection()
input.polling.set(input.cm.options.pollInterval,poll)}}
this.polling.set(this.cm.options.pollInterval,poll)};ContentEditableInput.prototype.selectionChanged=function selectionChanged(){var sel=window.getSelection()
return sel.anchorNode!=this.lastAnchorNode||sel.anchorOffset!=this.lastAnchorOffset||sel.focusNode!=this.lastFocusNode||sel.focusOffset!=this.lastFocusOffset};ContentEditableInput.prototype.pollSelection=function pollSelection(){if(this.readDOMTimeout!=null||this.gracePeriod||!this.selectionChanged()){return}
var sel=window.getSelection(),cm=this.cm
if(android&&chrome&&this.cm.options.gutters.length&&isInGutter(sel.anchorNode)){this.cm.triggerOnKeyDown({type:"keydown",keyCode:8,preventDefault:Math.abs})
this.blur()
this.focus()
return}
if(this.composing){return}
this.rememberSelection()
var anchor=domToPos(cm,sel.anchorNode,sel.anchorOffset)
var head=domToPos(cm,sel.focusNode,sel.focusOffset)
if(anchor&&head){runInOp(cm,function(){setSelection(cm.doc,simpleSelection(anchor,head),sel_dontScroll)
if(anchor.bad||head.bad){cm.curOp.selectionChanged=true}})}};ContentEditableInput.prototype.pollContent=function pollContent(){if(this.readDOMTimeout!=null){clearTimeout(this.readDOMTimeout)
this.readDOMTimeout=null}
var cm=this.cm,display=cm.display,sel=cm.doc.sel.primary()
var from=sel.from(),to=sel.to()
if(from.ch==0&&from.line>cm.firstLine())
{from=Pos(from.line-1,getLine(cm.doc,from.line-1).length)}
if(to.ch==getLine(cm.doc,to.line).text.length&&to.line<cm.lastLine())
{to=Pos(to.line+1,0)}
if(from.line<display.viewFrom||to.line>display.viewTo-1){return false}
var fromIndex,fromLine,fromNode
if(from.line==display.viewFrom||(fromIndex=findViewIndex(cm,from.line))==0){fromLine=lineNo(display.view[0].line)
fromNode=display.view[0].node}else{fromLine=lineNo(display.view[fromIndex].line)
fromNode=display.view[fromIndex-1].node.nextSibling}
var toIndex=findViewIndex(cm,to.line)
var toLine,toNode
if(toIndex==display.view.length-1){toLine=display.viewTo-1
toNode=display.lineDiv.lastChild}else{toLine=lineNo(display.view[toIndex+1].line)-1
toNode=display.view[toIndex+1].node.previousSibling}
if(!fromNode){return false}
var newText=cm.doc.splitLines(domTextBetween(cm,fromNode,toNode,fromLine,toLine))
var oldText=getBetween(cm.doc,Pos(fromLine,0),Pos(toLine,getLine(cm.doc,toLine).text.length))
while(newText.length>1&&oldText.length>1){if(lst(newText)==lst(oldText)){newText.pop();oldText.pop();toLine--}
else if(newText[0]==oldText[0]){newText.shift();oldText.shift();fromLine++}
else{break}}
var cutFront=0,cutEnd=0
var newTop=newText[0],oldTop=oldText[0],maxCutFront=Math.min(newTop.length,oldTop.length)
while(cutFront<maxCutFront&&newTop.charCodeAt(cutFront)==oldTop.charCodeAt(cutFront))
{++cutFront}
var newBot=lst(newText),oldBot=lst(oldText)
var maxCutEnd=Math.min(newBot.length-(newText.length==1?cutFront:0),oldBot.length-(oldText.length==1?cutFront:0))
while(cutEnd<maxCutEnd&&newBot.charCodeAt(newBot.length-cutEnd-1)==oldBot.charCodeAt(oldBot.length-cutEnd-1))
{++cutEnd}
if(newText.length==1&&oldText.length==1&&fromLine==from.line){while(cutFront&&cutFront>from.ch&&newBot.charCodeAt(newBot.length-cutEnd-1)==oldBot.charCodeAt(oldBot.length-cutEnd-1)){cutFront--
cutEnd++}}
newText[newText.length-1]=newBot.slice(0,newBot.length-cutEnd).replace(/^\u200b+/,"")
newText[0]=newText[0].slice(cutFront).replace(/\u200b+$/,"")
var chFrom=Pos(fromLine,cutFront)
var chTo=Pos(toLine,oldText.length?lst(oldText).length-cutEnd:0)
if(newText.length>1||newText[0]||cmp(chFrom,chTo)){replaceRange(cm.doc,newText,chFrom,chTo,"+input")
return true}};ContentEditableInput.prototype.ensurePolled=function ensurePolled(){this.forceCompositionEnd()};ContentEditableInput.prototype.reset=function reset(){this.forceCompositionEnd()};ContentEditableInput.prototype.forceCompositionEnd=function forceCompositionEnd(){if(!this.composing){return}
clearTimeout(this.readDOMTimeout)
this.composing=null
this.updateFromDOM()
this.div.blur()
this.div.focus()};ContentEditableInput.prototype.readFromDOMSoon=function readFromDOMSoon(){var this$1=this;if(this.readDOMTimeout!=null){return}
this.readDOMTimeout=setTimeout(function(){this$1.readDOMTimeout=null
if(this$1.composing){if(this$1.composing.done){this$1.composing=null}
else{return}}
this$1.updateFromDOM()},80)};ContentEditableInput.prototype.updateFromDOM=function updateFromDOM(){var this$1=this;if(this.cm.isReadOnly()||!this.pollContent())
{runInOp(this.cm,function(){return regChange(this$1.cm);})}};ContentEditableInput.prototype.setUneditable=function setUneditable(node){node.contentEditable="false"};ContentEditableInput.prototype.onKeyPress=function onKeyPress(e){if(e.charCode==0){return}
e.preventDefault()
if(!this.cm.isReadOnly())
{operation(this.cm,applyTextInput)(this.cm,String.fromCharCode(e.charCode==null?e.keyCode:e.charCode),0)}};ContentEditableInput.prototype.readOnlyChanged=function readOnlyChanged(val){this.div.contentEditable=String(val!="nocursor")};ContentEditableInput.prototype.onContextMenu=function onContextMenu(){};ContentEditableInput.prototype.resetPosition=function resetPosition(){};ContentEditableInput.prototype.needsContentAttribute=true
function posToDOM(cm,pos){var view=findViewForLine(cm,pos.line)
if(!view||view.hidden){return null}
var line=getLine(cm.doc,pos.line)
var info=mapFromLineView(view,line,pos.line)
var order=getOrder(line,cm.doc.direction),side="left"
if(order){var partPos=getBidiPartAt(order,pos.ch)
side=partPos%2?"right":"left"}
var result=nodeAndOffsetInLineMap(info.map,pos.ch,side)
result.offset=result.collapse=="right"?result.end:result.start
return result}
function isInGutter(node){for(var scan=node;scan;scan=scan.parentNode)
{if(/CodeMirror-gutter-wrapper/.test(scan.className)){return true}}
return false}
function badPos(pos,bad){if(bad){pos.bad=true;}return pos}
function domTextBetween(cm,from,to,fromLine,toLine){var text="",closing=false,lineSep=cm.doc.lineSeparator()
function recognizeMarker(id){return function(marker){return marker.id==id;}}
function close(){if(closing){text+=lineSep
closing=false}}
function addText(str){if(str){close()
text+=str}}
function walk(node){if(node.nodeType==1){var cmText=node.getAttribute("cm-text")
if(cmText!=null){addText(cmText||node.textContent.replace(/\u200b/g,""))
return}
var markerID=node.getAttribute("cm-marker"),range
if(markerID){var found=cm.findMarks(Pos(fromLine,0),Pos(toLine+1,0),recognizeMarker(+markerID))
if(found.length&&(range=found[0].find()))
{addText(getBetween(cm.doc,range.from,range.to).join(lineSep))}
return}
if(node.getAttribute("contenteditable")=="false"){return}
var isBlock=/^(pre|div|p)$/i.test(node.nodeName)
if(isBlock){close()}
for(var i=0;i<node.childNodes.length;i++)
{walk(node.childNodes[i])}
if(isBlock){closing=true}}else if(node.nodeType==3){addText(node.nodeValue)}}
for(;;){walk(from)
if(from==to){break}
from=from.nextSibling}
return text}
function domToPos(cm,node,offset){var lineNode
if(node==cm.display.lineDiv){lineNode=cm.display.lineDiv.childNodes[offset]
if(!lineNode){return badPos(cm.clipPos(Pos(cm.display.viewTo-1)),true)}
node=null;offset=0}else{for(lineNode=node;;lineNode=lineNode.parentNode){if(!lineNode||lineNode==cm.display.lineDiv){return null}
if(lineNode.parentNode&&lineNode.parentNode==cm.display.lineDiv){break}}}
for(var i=0;i<cm.display.view.length;i++){var lineView=cm.display.view[i]
if(lineView.node==lineNode)
{return locateNodeInLineView(lineView,node,offset)}}}
function locateNodeInLineView(lineView,node,offset){var wrapper=lineView.text.firstChild,bad=false
if(!node||!contains(wrapper,node)){return badPos(Pos(lineNo(lineView.line),0),true)}
if(node==wrapper){bad=true
node=wrapper.childNodes[offset]
offset=0
if(!node){var line=lineView.rest?lst(lineView.rest):lineView.line
return badPos(Pos(lineNo(line),line.text.length),bad)}}
var textNode=node.nodeType==3?node:null,topNode=node
if(!textNode&&node.childNodes.length==1&&node.firstChild.nodeType==3){textNode=node.firstChild
if(offset){offset=textNode.nodeValue.length}}
while(topNode.parentNode!=wrapper){topNode=topNode.parentNode}
var measure=lineView.measure,maps=measure.maps
function find(textNode,topNode,offset){for(var i=-1;i<(maps?maps.length:0);i++){var map=i<0?measure.map:maps[i]
for(var j=0;j<map.length;j+=3){var curNode=map[j+2]
if(curNode==textNode||curNode==topNode){var line=lineNo(i<0?lineView.line:lineView.rest[i])
var ch=map[j]+offset
if(offset<0||curNode!=textNode){ch=map[j+(offset?1:0)]}
return Pos(line,ch)}}}}
var found=find(textNode,topNode,offset)
if(found){return badPos(found,bad)}
for(var after=topNode.nextSibling,dist=textNode?textNode.nodeValue.length-offset:0;after;after=after.nextSibling){found=find(after,after.firstChild,0)
if(found)
{return badPos(Pos(found.line,found.ch-dist),bad)}
else
{dist+=after.textContent.length}}
for(var before=topNode.previousSibling,dist$1=offset;before;before=before.previousSibling){found=find(before,before.firstChild,-1)
if(found)
{return badPos(Pos(found.line,found.ch+dist$1),bad)}
else
{dist$1+=before.textContent.length}}}
var TextareaInput=function TextareaInput(cm){this.cm=cm
this.prevInput=""
this.pollingFast=false
this.polling=new Delayed()
this.inaccurateSelection=false
this.hasSelection=false
this.composing=null};TextareaInput.prototype.init=function init(display){var this$1=this;var input=this,cm=this.cm
var div=this.wrapper=hiddenTextarea()
var te=this.textarea=div.firstChild
display.wrapper.insertBefore(div,display.wrapper.firstChild)
if(ios){te.style.width="0px"}
on(te,"input",function(){if(ie&&ie_version>=9&&this$1.hasSelection){this$1.hasSelection=null}
input.poll()})
on(te,"paste",function(e){if(signalDOMEvent(cm,e)||handlePaste(e,cm)){return}
cm.state.pasteIncoming=true
input.fastPoll()})
function prepareCopyCut(e){if(signalDOMEvent(cm,e)){return}
if(cm.somethingSelected()){setLastCopied({lineWise:false,text:cm.getSelections()})
if(input.inaccurateSelection){input.prevInput=""
input.inaccurateSelection=false
te.value=lastCopied.text.join("\n")
selectInput(te)}}else if(!cm.options.lineWiseCopyCut){return}else{var ranges=copyableRanges(cm)
setLastCopied({lineWise:true,text:ranges.text})
if(e.type=="cut"){cm.setSelections(ranges.ranges,null,sel_dontScroll)}else{input.prevInput=""
te.value=ranges.text.join("\n")
selectInput(te)}}
if(e.type=="cut"){cm.state.cutIncoming=true}}
on(te,"cut",prepareCopyCut)
on(te,"copy",prepareCopyCut)
on(display.scroller,"paste",function(e){if(eventInWidget(display,e)||signalDOMEvent(cm,e)){return}
cm.state.pasteIncoming=true
input.focus()})
on(display.lineSpace,"selectstart",function(e){if(!eventInWidget(display,e)){e_preventDefault(e)}})
on(te,"compositionstart",function(){var start=cm.getCursor("from")
if(input.composing){input.composing.range.clear()}
input.composing={start:start,range:cm.markText(start,cm.getCursor("to"),{className:"CodeMirror-composing"})}})
on(te,"compositionend",function(){if(input.composing){input.poll()
input.composing.range.clear()
input.composing=null}})};TextareaInput.prototype.prepareSelection=function prepareSelection$1(){var cm=this.cm,display=cm.display,doc=cm.doc
var result=prepareSelection(cm)
if(cm.options.moveInputWithCursor){var headPos=cursorCoords(cm,doc.sel.primary().head,"div")
var wrapOff=display.wrapper.getBoundingClientRect(),lineOff=display.lineDiv.getBoundingClientRect()
result.teTop=Math.max(0,Math.min(display.wrapper.clientHeight-10,headPos.top+lineOff.top-wrapOff.top))
result.teLeft=Math.max(0,Math.min(display.wrapper.clientWidth-10,headPos.left+lineOff.left-wrapOff.left))}
return result};TextareaInput.prototype.showSelection=function showSelection(drawn){var cm=this.cm,display=cm.display
removeChildrenAndAdd(display.cursorDiv,drawn.cursors)
removeChildrenAndAdd(display.selectionDiv,drawn.selection)
if(drawn.teTop!=null){this.wrapper.style.top=drawn.teTop+"px"
this.wrapper.style.left=drawn.teLeft+"px"}};TextareaInput.prototype.reset=function reset(typing){if(this.contextMenuPending){return}
var minimal,selected,cm=this.cm,doc=cm.doc
if(cm.somethingSelected()){this.prevInput=""
var range=doc.sel.primary()
minimal=hasCopyEvent&&(range.to().line-range.from().line>100||(selected=cm.getSelection()).length>1000)
var content=minimal?"-":selected||cm.getSelection()
this.textarea.value=content
if(cm.state.focused){selectInput(this.textarea)}
if(ie&&ie_version>=9){this.hasSelection=content}}else if(!typing){this.prevInput=this.textarea.value=""
if(ie&&ie_version>=9){this.hasSelection=null}}
this.inaccurateSelection=minimal};TextareaInput.prototype.getField=function getField(){return this.textarea};TextareaInput.prototype.supportsTouch=function supportsTouch(){return false};TextareaInput.prototype.focus=function focus(){if(this.cm.options.readOnly!="nocursor"&&(!mobile||activeElt()!=this.textarea)){try{this.textarea.focus()}
catch(e){}}};TextareaInput.prototype.blur=function blur(){this.textarea.blur()};TextareaInput.prototype.resetPosition=function resetPosition(){this.wrapper.style.top=this.wrapper.style.left=0};TextareaInput.prototype.receivedFocus=function receivedFocus(){this.slowPoll()};TextareaInput.prototype.slowPoll=function slowPoll(){var this$1=this;if(this.pollingFast){return}
this.polling.set(this.cm.options.pollInterval,function(){this$1.poll()
if(this$1.cm.state.focused){this$1.slowPoll()}})};TextareaInput.prototype.fastPoll=function fastPoll(){var missed=false,input=this
input.pollingFast=true
function p(){var changed=input.poll()
if(!changed&&!missed){missed=true;input.polling.set(60,p)}
else{input.pollingFast=false;input.slowPoll()}}
input.polling.set(20,p)};TextareaInput.prototype.poll=function poll(){var this$1=this;var cm=this.cm,input=this.textarea,prevInput=this.prevInput
if(this.contextMenuPending||!cm.state.focused||(hasSelection(input)&&!prevInput&&!this.composing)||cm.isReadOnly()||cm.options.disableInput||cm.state.keySeq)
{return false}
var text=input.value
if(text==prevInput&&!cm.somethingSelected()){return false}
if(ie&&ie_version>=9&&this.hasSelection===text||mac&&/[\uf700-\uf7ff]/.test(text)){cm.display.input.reset()
return false}
if(cm.doc.sel==cm.display.selForContextMenu){var first=text.charCodeAt(0)
if(first==0x200b&&!prevInput){prevInput="\u200b"}
if(first==0x21da){this.reset();return this.cm.execCommand("undo")}}
var same=0,l=Math.min(prevInput.length,text.length)
while(same<l&&prevInput.charCodeAt(same)==text.charCodeAt(same)){++same}
runInOp(cm,function(){applyTextInput(cm,text.slice(same),prevInput.length-same,null,this$1.composing?"*compose":null)
if(text.length>1000||text.indexOf("\n")>-1){input.value=this$1.prevInput=""}
else{this$1.prevInput=text}
if(this$1.composing){this$1.composing.range.clear()
this$1.composing.range=cm.markText(this$1.composing.start,cm.getCursor("to"),{className:"CodeMirror-composing"})}})
return true};TextareaInput.prototype.ensurePolled=function ensurePolled(){if(this.pollingFast&&this.poll()){this.pollingFast=false}};TextareaInput.prototype.onKeyPress=function onKeyPress(){if(ie&&ie_version>=9){this.hasSelection=null}
this.fastPoll()};TextareaInput.prototype.onContextMenu=function onContextMenu(e){var input=this,cm=input.cm,display=cm.display,te=input.textarea
var pos=posFromMouse(cm,e),scrollPos=display.scroller.scrollTop
if(!pos||presto){return}
var reset=cm.options.resetSelectionOnContextMenu
if(reset&&cm.doc.sel.contains(pos)==-1)
{operation(cm,setSelection)(cm.doc,simpleSelection(pos),sel_dontScroll)}
var oldCSS=te.style.cssText,oldWrapperCSS=input.wrapper.style.cssText
input.wrapper.style.cssText="position: absolute"
var wrapperBox=input.wrapper.getBoundingClientRect()
te.style.cssText="position: absolute; width: 30px; height: 30px;\n      top: "+(e.clientY-wrapperBox.top-5)+"px; left: "+(e.clientX-wrapperBox.left-5)+"px;\n      z-index: 1000; background: "+(ie?"rgba(255, 255, 255, .05)":"transparent")+";\n      outline: none; border-width: 0; outline: none; overflow: hidden; opacity: .05; filter: alpha(opacity=5);"
var oldScrollY
if(webkit){oldScrollY=window.scrollY}
display.input.focus()
if(webkit){window.scrollTo(null,oldScrollY)}
display.input.reset()
if(!cm.somethingSelected()){te.value=input.prevInput=" "}
input.contextMenuPending=true
display.selForContextMenu=cm.doc.sel
clearTimeout(display.detectingSelectAll)
function prepareSelectAllHack(){if(te.selectionStart!=null){var selected=cm.somethingSelected()
var extval="\u200b"+(selected?te.value:"")
te.value="\u21da"
te.value=extval
input.prevInput=selected?"":"\u200b"
te.selectionStart=1;te.selectionEnd=extval.length
display.selForContextMenu=cm.doc.sel}}
function rehide(){input.contextMenuPending=false
input.wrapper.style.cssText=oldWrapperCSS
te.style.cssText=oldCSS
if(ie&&ie_version<9){display.scrollbars.setScrollTop(display.scroller.scrollTop=scrollPos)}
if(te.selectionStart!=null){if(!ie||(ie&&ie_version<9)){prepareSelectAllHack()}
var i=0,poll=function(){if(display.selForContextMenu==cm.doc.sel&&te.selectionStart==0&&te.selectionEnd>0&&input.prevInput=="\u200b"){operation(cm,selectAll)(cm)}else if(i++<10){display.detectingSelectAll=setTimeout(poll,500)}else{display.selForContextMenu=null
display.input.reset()}}
display.detectingSelectAll=setTimeout(poll,200)}}
if(ie&&ie_version>=9){prepareSelectAllHack()}
if(captureRightClick){e_stop(e)
var mouseup=function(){off(window,"mouseup",mouseup)
setTimeout(rehide,20)}
on(window,"mouseup",mouseup)}else{setTimeout(rehide,50)}};TextareaInput.prototype.readOnlyChanged=function readOnlyChanged(val){if(!val){this.reset()}};TextareaInput.prototype.setUneditable=function setUneditable(){};TextareaInput.prototype.needsContentAttribute=false
function fromTextArea(textarea,options){options=options?copyObj(options):{}
options.value=textarea.value
if(!options.tabindex&&textarea.tabIndex)
{options.tabindex=textarea.tabIndex}
if(!options.placeholder&&textarea.placeholder)
{options.placeholder=textarea.placeholder}
if(options.autofocus==null){var hasFocus=activeElt()
options.autofocus=hasFocus==textarea||textarea.getAttribute("autofocus")!=null&&hasFocus==document.body}
function save(){textarea.value=cm.getValue()}
var realSubmit
if(textarea.form){on(textarea.form,"submit",save)
if(!options.leaveSubmitMethodAlone){var form=textarea.form
realSubmit=form.submit
try{var wrappedSubmit=form.submit=function(){save()
form.submit=realSubmit
form.submit()
form.submit=wrappedSubmit}}catch(e){}}}
options.finishInit=function(cm){cm.save=save
cm.getTextArea=function(){return textarea;}
cm.toTextArea=function(){cm.toTextArea=isNaN
save()
textarea.parentNode.removeChild(cm.getWrapperElement())
textarea.style.display=""
if(textarea.form){off(textarea.form,"submit",save)
if(typeof textarea.form.submit=="function")
{textarea.form.submit=realSubmit}}}}
textarea.style.display="none"
var cm=CodeMirror(function(node){return textarea.parentNode.insertBefore(node,textarea.nextSibling);},options)
return cm}
function addLegacyProps(CodeMirror){CodeMirror.off=off
CodeMirror.on=on
CodeMirror.wheelEventPixels=wheelEventPixels
CodeMirror.Doc=Doc
CodeMirror.splitLines=splitLinesAuto
CodeMirror.countColumn=countColumn
CodeMirror.findColumn=findColumn
CodeMirror.isWordChar=isWordCharBasic
CodeMirror.Pass=Pass
CodeMirror.signal=signal
CodeMirror.Line=Line
CodeMirror.changeEnd=changeEnd
CodeMirror.scrollbarModel=scrollbarModel
CodeMirror.Pos=Pos
CodeMirror.cmpPos=cmp
CodeMirror.modes=modes
CodeMirror.mimeModes=mimeModes
CodeMirror.resolveMode=resolveMode
CodeMirror.getMode=getMode
CodeMirror.modeExtensions=modeExtensions
CodeMirror.extendMode=extendMode
CodeMirror.copyState=copyState
CodeMirror.startState=startState
CodeMirror.innerMode=innerMode
CodeMirror.commands=commands
CodeMirror.keyMap=keyMap
CodeMirror.keyName=keyName
CodeMirror.isModifierKey=isModifierKey
CodeMirror.lookupKey=lookupKey
CodeMirror.normalizeKeyMap=normalizeKeyMap
CodeMirror.StringStream=StringStream
CodeMirror.SharedTextMarker=SharedTextMarker
CodeMirror.TextMarker=TextMarker
CodeMirror.LineWidget=LineWidget
CodeMirror.e_preventDefault=e_preventDefault
CodeMirror.e_stopPropagation=e_stopPropagation
CodeMirror.e_stop=e_stop
CodeMirror.addClass=addClass
CodeMirror.contains=contains
CodeMirror.rmClass=rmClass
CodeMirror.keyNames=keyNames}
defineOptions(CodeMirror)
addEditorMethods(CodeMirror)
var dontDelegate="iter insert remove copy getEditor constructor".split(" ")
for(var prop in Doc.prototype){if(Doc.prototype.hasOwnProperty(prop)&&indexOf(dontDelegate,prop)<0)
{CodeMirror.prototype[prop]=(function(method){return function(){return method.apply(this.doc,arguments)}})(Doc.prototype[prop])}}
eventMixin(Doc)
CodeMirror.inputStyles={"textarea":TextareaInput,"contenteditable":ContentEditableInput}
CodeMirror.defineMode=function(name){if(!CodeMirror.defaults.mode&&name!="null"){CodeMirror.defaults.mode=name}
defineMode.apply(this,arguments)}
CodeMirror.defineMIME=defineMIME
CodeMirror.defineMode("null",function(){return({token:function(stream){return stream.skipToEnd();}});})
CodeMirror.defineMIME("text/plain","null")
CodeMirror.defineExtension=function(name,func){CodeMirror.prototype[name]=func}
CodeMirror.defineDocExtension=function(name,func){Doc.prototype[name]=func}
CodeMirror.fromTextArea=fromTextArea
addLegacyProps(CodeMirror)
CodeMirror.version="5.25.1"
return CodeMirror;})));;(function(mod){if(typeof exports=="object"&&typeof module=="object")
mod(require("../../lib/codemirror"));else if(typeof define=="function"&&define.amd)
define(["../../lib/codemirror"],mod);else
mod(CodeMirror);})(function(CodeMirror){"use strict";CodeMirror.multiplexingMode=function(outer){var others=Array.prototype.slice.call(arguments,1);function indexOf(string,pattern,from,returnEnd){if(typeof pattern=="string"){var found=string.indexOf(pattern,from);return returnEnd&&found>-1?found+pattern.length:found;}
var m=pattern.exec(from?string.slice(from):string);return m?m.index+from+(returnEnd?m[0].length:0):-1;}
return{startState:function(){return{outer:CodeMirror.startState(outer),innerActive:null,inner:null};},copyState:function(state){return{outer:CodeMirror.copyState(outer,state.outer),innerActive:state.innerActive,inner:state.innerActive&&CodeMirror.copyState(state.innerActive.mode,state.inner)};},token:function(stream,state){if(!state.innerActive){var cutOff=Infinity,oldContent=stream.string;for(var i=0;i<others.length;++i){var other=others[i];var found=indexOf(oldContent,other.open,stream.pos);if(found==stream.pos){if(!other.parseDelimiters)stream.match(other.open);state.innerActive=other;state.inner=CodeMirror.startState(other.mode,outer.indent?outer.indent(state.outer,""):0);return other.delimStyle&&(other.delimStyle+" "+other.delimStyle+"-open");}else if(found!=-1&&found<cutOff){cutOff=found;}}
if(cutOff!=Infinity)stream.string=oldContent.slice(0,cutOff);var outerToken=outer.token(stream,state.outer);if(cutOff!=Infinity)stream.string=oldContent;return outerToken;}else{var curInner=state.innerActive,oldContent=stream.string;if(!curInner.close&&stream.sol()){state.innerActive=state.inner=null;return this.token(stream,state);}
var found=curInner.close?indexOf(oldContent,curInner.close,stream.pos,curInner.parseDelimiters):-1;if(found==stream.pos&&!curInner.parseDelimiters){stream.match(curInner.close);state.innerActive=state.inner=null;return curInner.delimStyle&&(curInner.delimStyle+" "+curInner.delimStyle+"-close");}
if(found>-1)stream.string=oldContent.slice(0,found);var innerToken=curInner.mode.token(stream,state.inner);if(found>-1)stream.string=oldContent;if(found==stream.pos&&curInner.parseDelimiters)
state.innerActive=state.inner=null;if(curInner.innerStyle){if(innerToken)innerToken=innerToken+" "+curInner.innerStyle;else innerToken=curInner.innerStyle;}
return innerToken;}},indent:function(state,textAfter){var mode=state.innerActive?state.innerActive.mode:outer;if(!mode.indent)return CodeMirror.Pass;return mode.indent(state.innerActive?state.inner:state.outer,textAfter);},blankLine:function(state){var mode=state.innerActive?state.innerActive.mode:outer;if(mode.blankLine){mode.blankLine(state.innerActive?state.inner:state.outer);}
if(!state.innerActive){for(var i=0;i<others.length;++i){var other=others[i];if(other.open==="\n"){state.innerActive=other;state.inner=CodeMirror.startState(other.mode,mode.indent?mode.indent(state.outer,""):0);}}}else if(state.innerActive.close==="\n"){state.innerActive=state.inner=null;}},electricChars:outer.electricChars,innerMode:function(state){return state.inner?{state:state.inner,mode:state.innerActive.mode}:{state:state.outer,mode:outer};}};};});;(function(mod){if(typeof exports=="object"&&typeof module=="object")
mod(require("../../lib/codemirror"));else if(typeof define=="function"&&define.amd)
define(["../../lib/codemirror"],mod);else
mod(CodeMirror);})(function(CodeMirror){var ie_lt8=/MSIE \d/.test(navigator.userAgent)&&(document.documentMode==null||document.documentMode<8);var Pos=CodeMirror.Pos;var matching={"(":")>",")":"(<","[":"]>","]":"[<","{":"}>","}":"{<"};function findMatchingBracket(cm,where,strict,config){var line=cm.getLineHandle(where.line),pos=where.ch-1;var match=(pos>=0&&matching[line.text.charAt(pos)])||matching[line.text.charAt(++pos)];if(!match)return null;var dir=match.charAt(1)==">"?1:-1;if(strict&&(dir>0)!=(pos==where.ch))return null;var style=cm.getTokenTypeAt(Pos(where.line,pos+1));var found=scanForBracket(cm,Pos(where.line,pos+(dir>0?1:0)),dir,style||null,config);if(found==null)return null;return{from:Pos(where.line,pos),to:found&&found.pos,match:found&&found.ch==match.charAt(0),forward:dir>0};}
function scanForBracket(cm,where,dir,style,config){var maxScanLen=(config&&config.maxScanLineLength)||10000;var maxScanLines=(config&&config.maxScanLines)||1000;var stack=[];var re=config&&config.bracketRegex?config.bracketRegex:/[(){}[\]]/;var lineEnd=dir>0?Math.min(where.line+maxScanLines,cm.lastLine()+1):Math.max(cm.firstLine()-1,where.line-maxScanLines);for(var lineNo=where.line;lineNo!=lineEnd;lineNo+=dir){var line=cm.getLine(lineNo);if(!line)continue;var pos=dir>0?0:line.length-1,end=dir>0?line.length:-1;if(line.length>maxScanLen)continue;if(lineNo==where.line)pos=where.ch-(dir<0?1:0);for(;pos!=end;pos+=dir){var ch=line.charAt(pos);if(re.test(ch)&&(style===undefined||cm.getTokenTypeAt(Pos(lineNo,pos+1))==style)){var match=matching[ch];if((match.charAt(1)==">")==(dir>0))stack.push(ch);else if(!stack.length)return{pos:Pos(lineNo,pos),ch:ch};else stack.pop();}}}
return lineNo-dir==(dir>0?cm.lastLine():cm.firstLine())?false:null;}
function matchBrackets(cm,autoclear,config){var maxHighlightLen=cm.state.matchBrackets.maxHighlightLineLength||1000;var marks=[],ranges=cm.listSelections();for(var i=0;i<ranges.length;i++){var match=ranges[i].empty()&&findMatchingBracket(cm,ranges[i].head,false,config);if(match&&cm.getLine(match.from.line).length<=maxHighlightLen){var style=match.match?"CodeMirror-matchingbracket":"CodeMirror-nonmatchingbracket";marks.push(cm.markText(match.from,Pos(match.from.line,match.from.ch+1),{className:style}));if(match.to&&cm.getLine(match.to.line).length<=maxHighlightLen)
marks.push(cm.markText(match.to,Pos(match.to.line,match.to.ch+1),{className:style}));}}
if(marks.length){if(ie_lt8&&cm.state.focused)cm.focus();var clear=function(){cm.operation(function(){for(var i=0;i<marks.length;i++)marks[i].clear();});};if(autoclear)setTimeout(clear,800);else return clear;}}
var currentlyHighlighted=null;function doMatchBrackets(cm){cm.operation(function(){if(currentlyHighlighted){currentlyHighlighted();currentlyHighlighted=null;}
currentlyHighlighted=matchBrackets(cm,false,cm.state.matchBrackets);});}
CodeMirror.defineOption("matchBrackets",false,function(cm,val,old){if(old&&old!=CodeMirror.Init){cm.off("cursorActivity",doMatchBrackets);if(currentlyHighlighted){currentlyHighlighted();currentlyHighlighted=null;}}
if(val){cm.state.matchBrackets=typeof val=="object"?val:{};cm.on("cursorActivity",doMatchBrackets);}});CodeMirror.defineExtension("matchBrackets",function(){matchBrackets(this,true);});CodeMirror.defineExtension("findMatchingBracket",function(pos,strict,config){return findMatchingBracket(this,pos,strict,config);});CodeMirror.defineExtension("scanForBracket",function(pos,dir,style,config){return scanForBracket(this,pos,dir,style,config);});});;(function(mod){if(typeof exports=="object"&&typeof module=="object")
mod(require("../../lib/codemirror"));else if(typeof define=="function"&&define.amd)
define(["../../lib/codemirror"],mod);else
mod(CodeMirror);})(function(CodeMirror){var defaults={pairs:"()[]{}''\"\"",triples:"",explode:"[]{}"};var Pos=CodeMirror.Pos;CodeMirror.defineOption("autoCloseBrackets",false,function(cm,val,old){if(old&&old!=CodeMirror.Init){cm.removeKeyMap(keyMap);cm.state.closeBrackets=null;}
if(val){cm.state.closeBrackets=val;cm.addKeyMap(keyMap);}});function getOption(conf,name){if(name=="pairs"&&typeof conf=="string")return conf;if(typeof conf=="object"&&conf[name]!=null)return conf[name];return defaults[name];}
var bind=defaults.pairs+"`";var keyMap={Backspace:handleBackspace,Enter:handleEnter};for(var i=0;i<bind.length;i++)
keyMap["'"+bind.charAt(i)+"'"]=handler(bind.charAt(i));function handler(ch){return function(cm){return handleChar(cm,ch);};}
function getConfig(cm){var deflt=cm.state.closeBrackets;if(!deflt||deflt.override)return deflt;var mode=cm.getModeAt(cm.getCursor());return mode.closeBrackets||deflt;}
function handleBackspace(cm){var conf=getConfig(cm);if(!conf||cm.getOption("disableInput"))return CodeMirror.Pass;var pairs=getOption(conf,"pairs");var ranges=cm.listSelections();for(var i=0;i<ranges.length;i++){if(!ranges[i].empty())return CodeMirror.Pass;var around=charsAround(cm,ranges[i].head);if(!around||pairs.indexOf(around)%2!=0)return CodeMirror.Pass;}
for(var i=ranges.length-1;i>=0;i--){var cur=ranges[i].head;cm.replaceRange("",Pos(cur.line,cur.ch-1),Pos(cur.line,cur.ch+1),"+delete");}}
function handleEnter(cm){var conf=getConfig(cm);var explode=conf&&getOption(conf,"explode");if(!explode||cm.getOption("disableInput"))return CodeMirror.Pass;var ranges=cm.listSelections();for(var i=0;i<ranges.length;i++){if(!ranges[i].empty())return CodeMirror.Pass;var around=charsAround(cm,ranges[i].head);if(!around||explode.indexOf(around)%2!=0)return CodeMirror.Pass;}
cm.operation(function(){cm.replaceSelection("\n\n",null);cm.execCommand("goCharLeft");ranges=cm.listSelections();for(var i=0;i<ranges.length;i++){var line=ranges[i].head.line;cm.indentLine(line,null,true);cm.indentLine(line+1,null,true);}});}
function contractSelection(sel){var inverted=CodeMirror.cmpPos(sel.anchor,sel.head)>0;return{anchor:new Pos(sel.anchor.line,sel.anchor.ch+(inverted?-1:1)),head:new Pos(sel.head.line,sel.head.ch+(inverted?1:-1))};}
function handleChar(cm,ch){var conf=getConfig(cm);if(!conf||cm.getOption("disableInput"))return CodeMirror.Pass;var pairs=getOption(conf,"pairs");var pos=pairs.indexOf(ch);if(pos==-1)return CodeMirror.Pass;var triples=getOption(conf,"triples");var identical=pairs.charAt(pos+1)==ch;var ranges=cm.listSelections();var opening=pos%2==0;var type;for(var i=0;i<ranges.length;i++){var range=ranges[i],cur=range.head,curType;var next=cm.getRange(cur,Pos(cur.line,cur.ch+1));if(opening&&!range.empty()){curType="surround";}else if((identical||!opening)&&next==ch){if(identical&&stringStartsAfter(cm,cur))
curType="both";else if(triples.indexOf(ch)>=0&&cm.getRange(cur,Pos(cur.line,cur.ch+3))==ch+ch+ch)
curType="skipThree";else
curType="skip";}else if(identical&&cur.ch>1&&triples.indexOf(ch)>=0&&cm.getRange(Pos(cur.line,cur.ch-2),cur)==ch+ch&&(cur.ch<=2||cm.getRange(Pos(cur.line,cur.ch-3),Pos(cur.line,cur.ch-2))!=ch)){curType="addFour";}else if(identical){if(!CodeMirror.isWordChar(next)&&enteringString(cm,cur,ch))curType="both";else return CodeMirror.Pass;}else if(opening&&(cm.getLine(cur.line).length==cur.ch||isClosingBracket(next,pairs)||/\s/.test(next))){curType="both";}else{return CodeMirror.Pass;}
if(!type)type=curType;else if(type!=curType)return CodeMirror.Pass;}
var left=pos%2?pairs.charAt(pos-1):ch;var right=pos%2?ch:pairs.charAt(pos+1);cm.operation(function(){if(type=="skip"){cm.execCommand("goCharRight");}else if(type=="skipThree"){for(var i=0;i<3;i++)
cm.execCommand("goCharRight");}else if(type=="surround"){var sels=cm.getSelections();for(var i=0;i<sels.length;i++)
sels[i]=left+sels[i]+right;cm.replaceSelections(sels,"around");sels=cm.listSelections().slice();for(var i=0;i<sels.length;i++)
sels[i]=contractSelection(sels[i]);cm.setSelections(sels);}else if(type=="both"){cm.replaceSelection(left+right,null);cm.triggerElectric(left+right);cm.execCommand("goCharLeft");}else if(type=="addFour"){cm.replaceSelection(left+left+left+left,"before");cm.execCommand("goCharRight");}});}
function isClosingBracket(ch,pairs){var pos=pairs.lastIndexOf(ch);return pos>-1&&pos%2==1;}
function charsAround(cm,pos){var str=cm.getRange(Pos(pos.line,pos.ch-1),Pos(pos.line,pos.ch+1));return str.length==2?str:null;}
function enteringString(cm,pos,ch){var line=cm.getLine(pos.line);var token=cm.getTokenAt(pos);if(/\bstring2?\b/.test(token.type)||stringStartsAfter(cm,pos))return false;var stream=new CodeMirror.StringStream(line.slice(0,pos.ch)+ch+line.slice(pos.ch),4);stream.pos=stream.start=token.start;for(;;){var type1=cm.getMode().token(stream,token.state);if(stream.pos>=pos.ch+1)return/\bstring2?\b/.test(type1);stream.start=stream.pos;}}
function stringStartsAfter(cm,pos){var token=cm.getTokenAt(Pos(pos.line,pos.ch+1))
return/\bstring/.test(token.type)&&token.start==pos.ch}});;(function(mod){if(typeof exports=="object"&&typeof module=="object")
mod(require("../../lib/codemirror"));else if(typeof define=="function"&&define.amd)
define(["../../lib/codemirror"],mod);else
mod(CodeMirror);})(function(CodeMirror){"use strict";CodeMirror.defineOption("styleSelectedText",false,function(cm,val,old){var prev=old&&old!=CodeMirror.Init;if(val&&!prev){cm.state.markedSelection=[];cm.state.markedSelectionStyle=typeof val=="string"?val:"CodeMirror-selectedtext";reset(cm);cm.on("cursorActivity",onCursorActivity);cm.on("change",onChange);}else if(!val&&prev){cm.off("cursorActivity",onCursorActivity);cm.off("change",onChange);clear(cm);cm.state.markedSelection=cm.state.markedSelectionStyle=null;}});function onCursorActivity(cm){if(cm.state.markedSelection)
cm.operation(function(){update(cm);});}
function onChange(cm){if(cm.state.markedSelection&&cm.state.markedSelection.length)
cm.operation(function(){clear(cm);});}
var CHUNK_SIZE=8;var Pos=CodeMirror.Pos;var cmp=CodeMirror.cmpPos;function coverRange(cm,from,to,addAt){if(cmp(from,to)==0)return;var array=cm.state.markedSelection;var cls=cm.state.markedSelectionStyle;for(var line=from.line;;){var start=line==from.line?from:Pos(line,0);var endLine=line+CHUNK_SIZE,atEnd=endLine>=to.line;var end=atEnd?to:Pos(endLine,0);var mark=cm.markText(start,end,{className:cls});if(addAt==null)array.push(mark);else array.splice(addAt++,0,mark);if(atEnd)break;line=endLine;}}
function clear(cm){var array=cm.state.markedSelection;for(var i=0;i<array.length;++i)array[i].clear();array.length=0;}
function reset(cm){clear(cm);var ranges=cm.listSelections();for(var i=0;i<ranges.length;i++)
coverRange(cm,ranges[i].from(),ranges[i].to());}
function update(cm){if(!cm.somethingSelected())return clear(cm);if(cm.listSelections().length>1)return reset(cm);var from=cm.getCursor("start"),to=cm.getCursor("end");var array=cm.state.markedSelection;if(!array.length)return coverRange(cm,from,to);var coverStart=array[0].find(),coverEnd=array[array.length-1].find();if(!coverStart||!coverEnd||to.line-from.line<CHUNK_SIZE||cmp(from,coverEnd.to)>=0||cmp(to,coverStart.from)<=0)
return reset(cm);while(cmp(from,coverStart.from)>0){array.shift().clear();coverStart=array[0].find();}
if(cmp(from,coverStart.from)<0){if(coverStart.to.line-from.line<CHUNK_SIZE){array.shift().clear();coverRange(cm,from,coverStart.to,0);}else{coverRange(cm,from,coverStart.from,0);}}
while(cmp(to,coverEnd.to)<0){array.pop().clear();coverEnd=array[array.length-1].find();}
if(cmp(to,coverEnd.to)>0){if(to.line-coverEnd.from.line<CHUNK_SIZE){array.pop().clear();coverRange(cm,coverEnd.from,to);}else{coverRange(cm,coverEnd.to,to);}}}});;(function(mod){if(typeof exports=="object"&&typeof module=="object")
mod(require("../../lib/codemirror"));else if(typeof define=="function"&&define.amd)
define(["../../lib/codemirror"],mod);else
mod(CodeMirror);})(function(CodeMirror){"use strict";var noOptions={};var nonWS=/[^\s\u00a0]/;var Pos=CodeMirror.Pos;function firstNonWS(str){var found=str.search(nonWS);return found==-1?0:found;}
CodeMirror.commands.toggleComment=function(cm){cm.toggleComment();};CodeMirror.defineExtension("toggleComment",function(options){if(!options)options=noOptions;var cm=this;var minLine=Infinity,ranges=this.listSelections(),mode=null;for(var i=ranges.length-1;i>=0;i--){var from=ranges[i].from(),to=ranges[i].to();if(from.line>=minLine)continue;if(to.line>=minLine)to=Pos(minLine,0);minLine=from.line;if(mode==null){if(cm.uncomment(from,to,options))mode="un";else{cm.lineComment(from,to,options);mode="line";}}else if(mode=="un"){cm.uncomment(from,to,options);}else{cm.lineComment(from,to,options);}}});function probablyInsideString(cm,pos,line){return/\bstring\b/.test(cm.getTokenTypeAt(Pos(pos.line,0)))&&!/^[\'\"\`]/.test(line)}
function getMode(cm,pos){var mode=cm.getMode()
return mode.useInnerComments===false||!mode.innerMode?mode:cm.getModeAt(pos)}
CodeMirror.defineExtension("lineComment",function(from,to,options){if(!options)options=noOptions;var self=this,mode=getMode(self,from);var firstLine=self.getLine(from.line);if(firstLine==null||probablyInsideString(self,from,firstLine))return;var commentString=options.lineComment||mode.lineComment;if(!commentString){if(options.blockCommentStart||mode.blockCommentStart){options.fullLines=true;self.blockComment(from,to,options);}
return;}
var end=Math.min(to.ch!=0||to.line==from.line?to.line+1:to.line,self.lastLine()+1);var pad=options.padding==null?" ":options.padding;var blankLines=options.commentBlankLines||from.line==to.line;self.operation(function(){if(options.indent){var baseString=null;for(var i=from.line;i<end;++i){var line=self.getLine(i);var whitespace=line.slice(0,firstNonWS(line));if(baseString==null||baseString.length>whitespace.length){baseString=whitespace;}}
for(var i=from.line;i<end;++i){var line=self.getLine(i),cut=baseString.length;if(!blankLines&&!nonWS.test(line))continue;if(line.slice(0,cut)!=baseString)cut=firstNonWS(line);self.replaceRange(baseString+commentString+pad,Pos(i,0),Pos(i,cut));}}else{for(var i=from.line;i<end;++i){if(blankLines||nonWS.test(self.getLine(i)))
self.replaceRange(commentString+pad,Pos(i,0));}}});});CodeMirror.defineExtension("blockComment",function(from,to,options){if(!options)options=noOptions;var self=this,mode=getMode(self,from);var startString=options.blockCommentStart||mode.blockCommentStart;var endString=options.blockCommentEnd||mode.blockCommentEnd;if(!startString||!endString){if((options.lineComment||mode.lineComment)&&options.fullLines!=false)
self.lineComment(from,to,options);return;}
if(/\bcomment\b/.test(self.getTokenTypeAt(Pos(from.line,0))))return
var end=Math.min(to.line,self.lastLine());if(end!=from.line&&to.ch==0&&nonWS.test(self.getLine(end)))--end;var pad=options.padding==null?" ":options.padding;if(from.line>end)return;self.operation(function(){if(options.fullLines!=false){var lastLineHasText=nonWS.test(self.getLine(end));self.replaceRange(pad+endString,Pos(end));self.replaceRange(startString+pad,Pos(from.line,0));var lead=options.blockCommentLead||mode.blockCommentLead;if(lead!=null)for(var i=from.line+1;i<=end;++i)
if(i!=end||lastLineHasText)
self.replaceRange(lead+pad,Pos(i,0));}else{self.replaceRange(endString,to);self.replaceRange(startString,from);}});});CodeMirror.defineExtension("uncomment",function(from,to,options){if(!options)options=noOptions;var self=this,mode=getMode(self,from);var end=Math.min(to.ch!=0||to.line==from.line?to.line:to.line-1,self.lastLine()),start=Math.min(from.line,end);var lineString=options.lineComment||mode.lineComment,lines=[];var pad=options.padding==null?" ":options.padding,didSomething;lineComment:{if(!lineString)break lineComment;for(var i=start;i<=end;++i){var line=self.getLine(i);var found=line.indexOf(lineString);if(found>-1&&!/comment/.test(self.getTokenTypeAt(Pos(i,found+1))))found=-1;if(found==-1&&nonWS.test(line))break lineComment;if(found>-1&&nonWS.test(line.slice(0,found)))break lineComment;lines.push(line);}
self.operation(function(){for(var i=start;i<=end;++i){var line=lines[i-start];var pos=line.indexOf(lineString),endPos=pos+lineString.length;if(pos<0)continue;if(line.slice(endPos,endPos+pad.length)==pad)endPos+=pad.length;didSomething=true;self.replaceRange("",Pos(i,pos),Pos(i,endPos));}});if(didSomething)return true;}
var startString=options.blockCommentStart||mode.blockCommentStart;var endString=options.blockCommentEnd||mode.blockCommentEnd;if(!startString||!endString)return false;var lead=options.blockCommentLead||mode.blockCommentLead;var startLine=self.getLine(start),open=startLine.indexOf(startString)
if(open==-1)return false
var endLine=end==start?startLine:self.getLine(end)
var close=endLine.indexOf(endString,end==start?open+startString.length:0);if(close==-1&&start!=end){endLine=self.getLine(--end);close=endLine.indexOf(endString);}
var insideStart=Pos(start,open+1),insideEnd=Pos(end,close+1)
if(close==-1||!/comment/.test(self.getTokenTypeAt(insideStart))||!/comment/.test(self.getTokenTypeAt(insideEnd))||self.getRange(insideStart,insideEnd,"\n").indexOf(endString)>-1)
return false;var lastStart=startLine.lastIndexOf(startString,from.ch);var firstEnd=lastStart==-1?-1:startLine.slice(0,from.ch).indexOf(endString,lastStart+startString.length);if(lastStart!=-1&&firstEnd!=-1&&firstEnd+endString.length!=from.ch)return false;firstEnd=endLine.indexOf(endString,to.ch);var almostLastStart=endLine.slice(to.ch).lastIndexOf(startString,firstEnd-to.ch);lastStart=(firstEnd==-1||almostLastStart==-1)?-1:to.ch+almostLastStart;if(firstEnd!=-1&&lastStart!=-1&&lastStart!=to.ch)return false;self.operation(function(){self.replaceRange("",Pos(end,close-(pad&&endLine.slice(close-pad.length,close)==pad?pad.length:0)),Pos(end,close+endString.length));var openEnd=open+startString.length;if(pad&&startLine.slice(openEnd,openEnd+pad.length)==pad)openEnd+=pad.length;self.replaceRange("",Pos(start,open),Pos(start,openEnd));if(lead)for(var i=start+1;i<=end;++i){var line=self.getLine(i),found=line.indexOf(lead);if(found==-1||nonWS.test(line.slice(0,found)))continue;var foundEnd=found+lead.length;if(pad&&line.slice(foundEnd,foundEnd+pad.length)==pad)foundEnd+=pad.length;self.replaceRange("",Pos(i,found),Pos(i,foundEnd));}});return true;});});;(function(mod){if(typeof exports=="object"&&typeof module=="object")
mod(require("../../lib/codemirror"));else if(typeof define=="function"&&define.amd)
define(["../../lib/codemirror"],mod);else
mod(CodeMirror);})(function(CodeMirror){"use strict";CodeMirror.overlayMode=function(base,overlay,combine){return{startState:function(){return{base:CodeMirror.startState(base),overlay:CodeMirror.startState(overlay),basePos:0,baseCur:null,overlayPos:0,overlayCur:null,streamSeen:null};},copyState:function(state){return{base:CodeMirror.copyState(base,state.base),overlay:CodeMirror.copyState(overlay,state.overlay),basePos:state.basePos,baseCur:null,overlayPos:state.overlayPos,overlayCur:null};},token:function(stream,state){if(stream!=state.streamSeen||Math.min(state.basePos,state.overlayPos)<stream.start){state.streamSeen=stream;state.basePos=state.overlayPos=stream.start;}
if(stream.start==state.basePos){state.baseCur=base.token(stream,state.base);state.basePos=stream.pos;}
if(stream.start==state.overlayPos){stream.pos=stream.start;state.overlayCur=overlay.token(stream,state.overlay);state.overlayPos=stream.pos;}
stream.pos=Math.min(state.basePos,state.overlayPos);if(state.overlayCur==null)return state.baseCur;else if(state.baseCur!=null&&state.overlay.combineTokens||combine&&state.overlay.combineTokens==null)
return state.baseCur+" "+state.overlayCur;else return state.overlayCur;},indent:base.indent&&function(state,textAfter){return base.indent(state.base,textAfter);},electricChars:base.electricChars,innerMode:function(state){return{state:state.base,mode:base};},blankLine:function(state){var baseToken,overlayToken;if(base.blankLine)baseToken=base.blankLine(state.base);if(overlay.blankLine)overlayToken=overlay.blankLine(state.overlay);return overlayToken==null?baseToken:(combine&&baseToken!=null?baseToken+" "+overlayToken:overlayToken);}};};});;(function(mod){if(typeof exports=="object"&&typeof module=="object")
mod(require("../../lib/codemirror"));else if(typeof define=="function"&&define.amd)
define(["../../lib/codemirror"],mod);else
mod(CodeMirror);})(function(CodeMirror){"use strict";var WRAP_CLASS="CodeMirror-activeline";var BACK_CLASS="CodeMirror-activeline-background";var GUTT_CLASS="CodeMirror-activeline-gutter";CodeMirror.defineOption("styleActiveLine",false,function(cm,val,old){var prev=old==CodeMirror.Init?false:old;if(val==prev)return
if(prev){cm.off("beforeSelectionChange",selectionChange);clearActiveLines(cm);delete cm.state.activeLines;}
if(val){cm.state.activeLines=[];updateActiveLines(cm,cm.listSelections());cm.on("beforeSelectionChange",selectionChange);}});function clearActiveLines(cm){for(var i=0;i<cm.state.activeLines.length;i++){cm.removeLineClass(cm.state.activeLines[i],"wrap",WRAP_CLASS);cm.removeLineClass(cm.state.activeLines[i],"background",BACK_CLASS);cm.removeLineClass(cm.state.activeLines[i],"gutter",GUTT_CLASS);}}
function sameArray(a,b){if(a.length!=b.length)return false;for(var i=0;i<a.length;i++)
if(a[i]!=b[i])return false;return true;}
function updateActiveLines(cm,ranges){var active=[];for(var i=0;i<ranges.length;i++){var range=ranges[i];var option=cm.getOption("styleActiveLine");if(typeof option=="object"&&option.nonEmpty?range.anchor.line!=range.head.line:!range.empty())
continue
var line=cm.getLineHandleVisualStart(range.head.line);if(active[active.length-1]!=line)active.push(line);}
if(sameArray(cm.state.activeLines,active))return;cm.operation(function(){clearActiveLines(cm);for(var i=0;i<active.length;i++){cm.addLineClass(active[i],"wrap",WRAP_CLASS);cm.addLineClass(active[i],"background",BACK_CLASS);cm.addLineClass(active[i],"gutter",GUTT_CLASS);}
cm.state.activeLines=active;});}
function selectionChange(cm,sel){updateActiveLines(cm,sel.ranges);}});;Runtime.cachedResources["cm/codemirror.css"]="/* BASICS */\n\n.CodeMirror {\n  /* Set height, width, borders, and global font properties here */\n  font-family: monospace;\n  height: 300px;\n  color: black;\n}\n\n/* PADDING */\n\n.CodeMirror-lines {\n  padding: 4px 0; /* Vertical padding around content */\n}\n.CodeMirror pre {\n  padding: 0 4px; /* Horizontal padding of content */\n}\n\n.CodeMirror-scrollbar-filler, .CodeMirror-gutter-filler {\n  background-color: white; /* The little square between H and V scrollbars */\n}\n\n/* GUTTER */\n\n.CodeMirror-gutters {\n  border-right: 1px solid #ddd;\n  background-color: #f7f7f7;\n  white-space: nowrap;\n}\n.CodeMirror-linenumbers {}\n.CodeMirror-linenumber {\n  padding: 0 3px 0 5px;\n  min-width: 20px;\n  text-align: right;\n  color: #999;\n  white-space: nowrap;\n}\n\n.CodeMirror-guttermarker { color: black; }\n.CodeMirror-guttermarker-subtle { color: #999; }\n\n/* CURSOR */\n\n.CodeMirror-cursor {\n  border-left: 1px solid black;\n  border-right: none;\n  width: 0;\n}\n/* Shown when moving in bi-directional text */\n.CodeMirror div.CodeMirror-secondarycursor {\n  border-left: 1px solid silver;\n}\n.cm-fat-cursor .CodeMirror-cursor {\n  width: auto;\n  border: 0 !important;\n  background: #7e7;\n}\n.cm-fat-cursor div.CodeMirror-cursors {\n  z-index: 1;\n}\n\n.cm-animate-fat-cursor {\n  width: auto;\n  border: 0;\n  -webkit-animation: blink 1.06s steps(1) infinite;\n  -moz-animation: blink 1.06s steps(1) infinite;\n  animation: blink 1.06s steps(1) infinite;\n  background-color: #7e7;\n}\n@-moz-keyframes blink {\n  0% {}\n  50% { background-color: transparent; }\n  100% {}\n}\n@-webkit-keyframes blink {\n  0% {}\n  50% { background-color: transparent; }\n  100% {}\n}\n@keyframes blink {\n  0% {}\n  50% { background-color: transparent; }\n  100% {}\n}\n\n/* Can style cursor different in overwrite (non-insert) mode */\n.CodeMirror-overwrite .CodeMirror-cursor {}\n\n.cm-tab { display: inline-block; text-decoration: inherit; }\n\n.CodeMirror-rulers {\n  position: absolute;\n  left: 0; right: 0; top: -50px; bottom: -20px;\n  overflow: hidden;\n}\n.CodeMirror-ruler {\n  border-left: 1px solid #ccc;\n  top: 0; bottom: 0;\n  position: absolute;\n}\n\n/* DEFAULT THEME */\n\n.cm-s-default .cm-header {color: blue;}\n.cm-s-default .cm-quote {color: #090;}\n.cm-negative {color: #d44;}\n.cm-positive {color: #292;}\n.cm-header, .cm-strong {font-weight: bold;}\n.cm-em {font-style: italic;}\n.cm-link {text-decoration: underline;}\n.cm-strikethrough {text-decoration: line-through;}\n\n.cm-s-default .cm-keyword {color: #708;}\n.cm-s-default .cm-atom {color: #219;}\n.cm-s-default .cm-number {color: #164;}\n.cm-s-default .cm-def {color: #00f;}\n.cm-s-default .cm-variable,\n.cm-s-default .cm-punctuation,\n.cm-s-default .cm-property,\n.cm-s-default .cm-operator {}\n.cm-s-default .cm-variable-2 {color: #05a;}\n.cm-s-default .cm-variable-3 {color: #085;}\n.cm-s-default .cm-comment {color: #a50;}\n.cm-s-default .cm-string {color: #a11;}\n.cm-s-default .cm-string-2 {color: #f50;}\n.cm-s-default .cm-meta {color: #555;}\n.cm-s-default .cm-qualifier {color: #555;}\n.cm-s-default .cm-builtin {color: #30a;}\n.cm-s-default .cm-bracket {color: #997;}\n.cm-s-default .cm-tag {color: #170;}\n.cm-s-default .cm-attribute {color: #00c;}\n.cm-s-default .cm-hr {color: #999;}\n.cm-s-default .cm-link {color: #00c;}\n\n.cm-s-default .cm-error {color: #f00;}\n.cm-invalidchar {color: #f00;}\n\n.CodeMirror-composing { border-bottom: 2px solid; }\n\n/* Default styles for common addons */\n\ndiv.CodeMirror span.CodeMirror-matchingbracket {color: #0f0;}\ndiv.CodeMirror span.CodeMirror-nonmatchingbracket {color: #f22;}\n.CodeMirror-matchingtag { background: rgba(255, 150, 0, .3); }\n.CodeMirror-activeline-background {background: #e8f2ff;}\n\n/* STOP */\n\n/* The rest of this file contains styles related to the mechanics of\n   the editor. You probably shouldn't touch them. */\n\n.CodeMirror {\n  position: relative;\n  overflow: hidden;\n  background: white;\n}\n\n.CodeMirror-scroll {\n  overflow: scroll !important; /* Things will break if this is overridden */\n  /* 30px is the magic margin used to hide the element's real scrollbars */\n  /* See overflow: hidden in .CodeMirror */\n  margin-bottom: -30px; margin-right: -30px;\n  padding-bottom: 30px;\n  height: 100%;\n  outline: none; /* Prevent dragging from highlighting the element */\n  position: relative;\n}\n.CodeMirror-sizer {\n  position: relative;\n  border-right: 30px solid transparent;\n}\n\n/* The fake, visible scrollbars. Used to force redraw during scrolling\n   before actual scrolling happens, thus preventing shaking and\n   flickering artifacts. */\n.CodeMirror-vscrollbar, .CodeMirror-hscrollbar, .CodeMirror-scrollbar-filler, .CodeMirror-gutter-filler {\n  position: absolute;\n  z-index: 6;\n  display: none;\n}\n.CodeMirror-vscrollbar {\n  right: 0; top: 0;\n  overflow-x: hidden;\n  overflow-y: scroll;\n}\n.CodeMirror-hscrollbar {\n  bottom: 0; left: 0;\n  overflow-y: hidden;\n  overflow-x: scroll;\n}\n.CodeMirror-scrollbar-filler {\n  right: 0; bottom: 0;\n}\n.CodeMirror-gutter-filler {\n  left: 0; bottom: 0;\n}\n\n.CodeMirror-gutters {\n  position: absolute; left: 0; top: 0;\n  min-height: 100%;\n  z-index: 3;\n}\n.CodeMirror-gutter {\n  white-space: normal;\n  height: 100%;\n  display: inline-block;\n  vertical-align: top;\n  margin-bottom: -30px;\n}\n.CodeMirror-gutter-wrapper {\n  position: absolute;\n  z-index: 4;\n  background: none !important;\n  border: none !important;\n}\n.CodeMirror-gutter-background {\n  position: absolute;\n  top: 0; bottom: 0;\n  z-index: 4;\n}\n.CodeMirror-gutter-elt {\n  position: absolute;\n  cursor: default;\n  z-index: 4;\n}\n.CodeMirror-gutter-wrapper ::selection { background-color: transparent }\n.CodeMirror-gutter-wrapper ::-moz-selection { background-color: transparent }\n\n.CodeMirror-lines {\n  cursor: text;\n  min-height: 1px; /* prevents collapsing before first draw */\n}\n.CodeMirror pre {\n  /* Reset some styles that the rest of the page might have set */\n  -moz-border-radius: 0; -webkit-border-radius: 0; border-radius: 0;\n  border-width: 0;\n  background: transparent;\n  font-family: inherit;\n  font-size: inherit;\n  margin: 0;\n  white-space: pre;\n  word-wrap: normal;\n  line-height: inherit;\n  color: inherit;\n  z-index: 2;\n  position: relative;\n  overflow: visible;\n  -webkit-tap-highlight-color: transparent;\n  -webkit-font-variant-ligatures: contextual;\n  font-variant-ligatures: contextual;\n}\n.CodeMirror-wrap pre {\n  word-wrap: break-word;\n  white-space: pre-wrap;\n  word-break: normal;\n}\n\n.CodeMirror-linebackground {\n  position: absolute;\n  left: 0; right: 0; top: 0; bottom: 0;\n  z-index: 0;\n}\n\n.CodeMirror-linewidget {\n  position: relative;\n  z-index: 2;\n  overflow: auto;\n}\n\n.CodeMirror-widget {}\n\n.CodeMirror-rtl pre { direction: rtl; }\n\n.CodeMirror-code {\n  outline: none;\n}\n\n/* Force content-box sizing for the elements where we expect it */\n.CodeMirror-scroll,\n.CodeMirror-sizer,\n.CodeMirror-gutter,\n.CodeMirror-gutters,\n.CodeMirror-linenumber {\n  -moz-box-sizing: content-box;\n  box-sizing: content-box;\n}\n\n.CodeMirror-measure {\n  position: absolute;\n  width: 100%;\n  height: 0;\n  overflow: hidden;\n  visibility: hidden;\n}\n\n.CodeMirror-cursor {\n  position: absolute;\n  pointer-events: none;\n}\n.CodeMirror-measure pre { position: static; }\n\ndiv.CodeMirror-cursors {\n  visibility: hidden;\n  position: relative;\n  z-index: 3;\n}\ndiv.CodeMirror-dragcursors {\n  visibility: visible;\n}\n\n.CodeMirror-focused div.CodeMirror-cursors {\n  visibility: visible;\n}\n\n.CodeMirror-selected { background: #d9d9d9; }\n.CodeMirror-focused .CodeMirror-selected { background: #d7d4f0; }\n.CodeMirror-crosshair { cursor: crosshair; }\n.CodeMirror-line::selection, .CodeMirror-line > span::selection, .CodeMirror-line > span > span::selection { background: #d7d4f0; }\n.CodeMirror-line::-moz-selection, .CodeMirror-line > span::-moz-selection, .CodeMirror-line > span > span::-moz-selection { background: #d7d4f0; }\n\n.cm-searching {\n  background: #ffa;\n  background: rgba(255, 255, 0, .4);\n}\n\n/* Used to force a border model for a node */\n.cm-force-border { padding-right: .1px; }\n\n@media print {\n  /* Hide the cursor when printing */\n  .CodeMirror div.CodeMirror-cursors {\n    visibility: hidden;\n  }\n}\n\n/* See issue #2901 */\n.cm-tab-wrap-hack:after { content: ''; }\n\n/* Help users use markselection to safely style text background */\nspan.CodeMirror-selectedtext { background: none; }\n\n/*# sourceURL=cm/codemirror.css */";