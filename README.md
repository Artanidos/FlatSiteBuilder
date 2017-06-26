# FlatSiteBuilder
The desktop app is working as a content mangement system and is creating a flat file web content to use with github pages.

# Syntax
The syntax for the templates is a mixture of Django, Markdown and Yaml and inspired by Jekyll.

## Variable
{{ varname }}

## Includes
{% include filename %}

## Loop
{% for page in pages %}
    {{ page.title }}
{% endfor %}
