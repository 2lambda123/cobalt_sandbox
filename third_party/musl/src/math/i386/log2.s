.global log2
.type log2,@function
log2:
	fld1
	fldl 4(%esp)
	fyl2x
	ret
