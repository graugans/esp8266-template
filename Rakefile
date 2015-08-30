require 'rakeoe'

# Override some defaults
config = RakeOE::Config.new

config.optimization_release = '-Os'
# we have no debugger so we need no debug optimization
config.optimization_dbg = '-Os'
config.generate_bin = false
config.generate_hex = false
config.generate_map = true

# dump config
config.dump

tool = RakeOE::init(config)
