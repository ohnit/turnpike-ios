require 'pandoc-ruby'

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
	File.open("source/index.html", "w") do |file|
		File.open("source/javascript_includes.txt").each do |line|
			file << line 
		end
		File.open("source/index-draft.html").each do |line|
			file << line 
		end
		File.open("source/end.txt").each do |line|
			file << line 
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



