from jinja2 import Template

def translate(code, context):
	template = Template(code)
	return template.render(context)
