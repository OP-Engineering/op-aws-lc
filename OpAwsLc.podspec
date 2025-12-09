require "json"

package = JSON.parse(File.read(File.join(__dir__, "package.json")))

Pod::Spec.new do |s|
  s.name         = "OpAwsLc"
  s.version      = package["version"]
  s.summary      = package["description"]
  s.homepage     = package["homepage"]
  s.license      = package["license"]
  s.authors      = package["author"]

  s.platforms    = { :ios => min_ios_version_supported }
  s.source       = { :git => "https://github.com/OP-Engineering/op-aws-lc.git", :tag => "#{s.version}" }

  s.source_files = Dir.glob("ios/**/*.{h,hpp,m,mm}") + Dir.glob("cpp/**/*.{hpp,h,cpp,c}")
  s.private_header_files = "ios/**/*.h"
  s.vendored_frameworks = "ios/*.xcframework"
  s.pod_target_xcconfig = {
    'EXCLUDED_ARCHS[sdk=iphonesimulator*]' => 'x86_64'
  }

  install_modules_dependencies(s)
end
