import jinja2

def translateTemplate(dir, template, globals, pagevars):
	loader = jinja2.FileSystemLoader([dir + globals["site"]["theme"] + "/layouts", dir + globals["site"]["theme"] + "/includes"])
	environment = jinja2.Environment(loader=loader)
	template = environment.get_template(template, None, pagevars)
	return template.render(globals)

def translateContent(code, globals, pagevars):
	environment = jinja2.Environment()
	template = environment.from_string(code, pagevars)
	return template.render(globals)
