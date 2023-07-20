commit ?= test

push:
	@git add .
	@git commit -m "$(commit)"
	@git push
	@sync #sync all files in os

