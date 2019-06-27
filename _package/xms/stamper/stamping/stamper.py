from .._xmsstamper.stamper import stamper
from .stamper_io import StamperIo

def stamp(stamper_io):
    """

    Args:
        stamper_io:

    Returns:

    """
    if not isinstance(stamper_io, StamperIo):
        raise ValueError("input must be of type StamperIo")
    stamper.stamp(stamper_io._instance)
