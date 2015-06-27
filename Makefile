### Makefile --- 

## Author: wkretzsch@gmail.com
## Version: $Id: Makefile,v 0.0 2015/06/27 08:38:21 winni Exp $
## Keywords: 
## X-URL: 

all:
	$(MAKE) -C gtest-1.7.0

test: all
	$(MAKE) -C testing

### Makefile ends here
