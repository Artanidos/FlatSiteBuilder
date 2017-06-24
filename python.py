import jinja2

def translate(dir, template, context):
	template_dir = dir
	loader = jinja2.FileSystemLoader(template_dir)
	environment = jinja2.Environment(loader=loader)
	template = environment.get_template(template)
	return template.render(context)
