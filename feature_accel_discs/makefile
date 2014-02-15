IPADDR = 158.130.110.163
all: build-all upload
git:
	git pull
	git add .
	git commit
	git push
build-all:
	../sdk/bin/pebble build
upload:
	../sdk/bin/pebble install --phone $(IPADDR)
