import jinja2
import mistune
import yaml

def translateTemplate(dir, template, globals, pagevars):
	loader = jinja2.FileSystemLoader([dir + "/themes/" + globals["site"]["theme"], dir + "/themes/" + globals["site"]["theme"] + "/includes"])
	environment = jinja2.Environment(loader=loader)
	template = environment.get_template(template, None, pagevars)
	return template.render(globals)

def translateContent(code, globals, pagevars):
	environment = jinja2.Environment()
	template = environment.from_string(code, pagevars)
	return template.render(globals)

def translateMarkdown(code):
	return mistune.markdown(code, escape=False, hard_wrap=True)

def parseYaml(code):
    return yaml.load(code)
