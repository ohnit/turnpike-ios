require 'pandoc-ruby'
require 'less'
require 'erb'

class CSSData
	def initialize(css_data)
		@css_data = css_data
	end
	def get_binding
		binding
	end
end

def read_in_file(file_name, compiled_rst)
	File.open("source/#{file_name}").each do |line|
		compiled_rst << line
	end
end

def convert_to_html(file_name)
	PandocRuby.allow_file_paths = true
	html = PandocRuby.rst('source/compiled.rst').to_html
	File.open("source/index-draft.html", "w") do |index|
		index << html
	end
end

def add_tocify
	File.open("index.html", "w") do |file|
		less_parser = Less::Parser.new
		css_data = CSSData.new(less_parser.parse(File.read('source/style.css.less')).to_css)

		File.open("source/beginning.html.erb") do |line|
			file << ERB.new(line.read).result(css_data.get_binding())
		end
		File.open("source/index-draft.html") do |line|
			file << line.read
		end
		File.open("source/end.html") do |line|
			file << line.read 
		end
	end
end

compiled_rst = File.open("source/compiled.rst", "w") do |compiled_rst|
	file_names = ['index.rst', 'installation.rst', 'enabling-mobile-deeplinking.rst', 'mapping-routes.rst', 'route-requests.rst', 'filter-chains.rst']
	file_names.each do |file_name|
   		read_in_file(file_name, compiled_rst)
   	end
   	html_file = convert_to_html('word')
   	add_tocify
end



