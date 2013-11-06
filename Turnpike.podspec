Pod::Spec.new do |s|
  s.name         = "Turnpike"
  s.version      = "0.0.1"
  s.summary      = "An iOS Router to enable deeplinking in mobile apps."
  s.description  = <<-DESC
                    Turnpike enables routing and filtering of incoming URLs to your app.
                   DESC
  s.homepage     = "http://urxtech.github.io/turnpike-ios/"
  s.license      = { :type => 'Apache', :file => 'LICENSE.txt' }
  s.author       = { "James Lawrence Turner" => "james@jameslawrenceturner.com" }
  s.source       = { :git => "https://github.com/URXtech/turnpike-ios.git", :commit => "f253c5abfa71feeae2f548d343464af138700404" }
  s.platform     = :ios
  s.source_files = 'Turnpike'
  s.requires_arc = true
end
