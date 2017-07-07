# FlatSiteBuilder

![Image](FlatSiteBuilder_very_early.png)

The desktop app is working as a content mangement system and is creating a flat file web content to use with github pages.

# Syntax
The syntax for the templates is based on Django.

## Variable
{{ varname }}

## Includes
{% include filename %}

## Loop
{% for page in pages %}
    {{ page.title }}
{% endfor %}
