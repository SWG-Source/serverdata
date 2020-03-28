# Clientdata Repository

This is the server side client data repo ONLY. DO NOT include files in this repo that would normally be used ONLY on the client side.  Store only files necessary for the SERVER operation here.  This would exclude files like sound files and MOST appearance files that do not have collision.

OPTIONAL: Compile into SOT files (see tre-sot-builders repo or ask Erusman - thanks dude!). Alternatively, use as-is.

# Usage

You should 

    cd /path/to/swg/data/sku.0/sys.client/compiled

Then clone this repo. You should also remove any other sku.[1-3] or sku.ja folders you may have inside data, as those are also client only. You'll then need to edit your exe/shared/servercommon.cfg to remove those old, dead paths.

# Useful Changes Appreciated

If you make any useful changes or improvements, put in a pull request/merge request and I'll merge it in, and you'll become a contributor!

