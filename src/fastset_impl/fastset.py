def __bootstrap__():
    import sys
    import os
    from hpy.universal import load

    f = __file__

    if os.path.exists(os.path.join(os.path.dirname(f), "fastset.hpy.so")):
        ext_filepath = os.path.join(os.path.dirname(f), "fastset.hpy.so")
    else:
        ext_filepath = os.path.join(os.path.dirname(os.path.dirname(f)), "fastset.hpy.so")

    m = load('fastset', ext_filepath, debug=False)
    m.__file__ = ext_filepath
    m.__loader__ = __loader__
    m.__name__ = __name__
    m.__package__ = __package__
    m.__spec__ = __spec__
    m.__spec__.origin = ext_filepath
    sys.modules[__name__] = m


__bootstrap__()
