marp := marp

SLIDES_DIR := content/slides

slides:
	$(marp) $(SLIDES_DIR)/*/*.md

commit ?= test

push: slides
	@git add .
	@git commit -m "$(commit)"
	@git push
	@sync #sync all files in os
