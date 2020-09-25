"""Method for performing a stamping operation."""
from .stamper_io import StamperIo
from .._xmsstamper.stamper import stamper


def stamp(stamper_io):
    """Performs the stamp using the options set in stamper_io.

    Args:
        stamper_io (:obj:`StamperIo <xms.stamper.stamping.StamperIo>`): options and settings used for stamping
    """
    if not isinstance(stamper_io, StamperIo):
        raise ValueError("input must be of type StamperIo")
    stamper.stamp(stamper_io._instance)
