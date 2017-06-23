# FlatSiteBuilder
The desktop app is working as a content mangement system and is creating a flat file web content to use with github pages.

# Syntax
Tha syntax comes from the Django projects

## Variable
{{ varname }}

## Includes
{% include filename %}

## Loop
{% for page in pages %}
    {{ page.title }}
{% endfor %}
