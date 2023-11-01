commit ?= test

# your github username
username := wlonestar

commit:
	@git add .
	@git commit -m "$(commit)"

remote: commit
	@git remote add origin \
		git@github.com:$(username)/$(username).github.io.git
	@git push -u origin master

push: commit
	@git push
	@sync #sync all files in os

