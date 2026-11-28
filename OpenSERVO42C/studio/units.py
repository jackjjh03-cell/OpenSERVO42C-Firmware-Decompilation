"""Display conversion only. The controller wire protocol remains in encoder counts."""
from decimal import Decimal, InvalidOperation, localcontext

UNITS=('Counts','Degrees','Revolutions','Millimeters','Inches')


def scale(unit, lead=''):
    if unit not in UNITS:
        raise ValueError('Unknown position unit')
    if unit=='Counts':return Decimal(1)
    if unit=='Degrees':return Decimal(360)/65536
    if unit=='Revolutions':return Decimal(1)/65536
    try:
        mm=Decimal(lead)
    except (InvalidOperation, ValueError):
        raise ValueError('Enter measured millimeters per motor revolution') from None
    if not mm.is_finite() or mm<=0 or mm>Decimal('1000000') or mm<Decimal('0.000001'):
        raise ValueError('Enter a positive finite travel per motor revolution (mm)')
    return mm/65536 if unit=='Millimeters' else mm/Decimal('25.4')/65536


def readout(actual,target,speed,unit,lead=''):
    factor=scale(unit,lead)
    if unit=='Counts':return 'Actual {:,} | Target {:,} | Speed {:,} counts/s'.format(actual,target,speed)
    suffix={'Degrees':'deg','Revolutions':'rev','Millimeters':'mm','Inches':'in'}[unit]
    with localcontext() as ctx:
        ctx.prec=32
        return 'Actual {:.4f} {} | Target {:.4f} {} | Speed {:.4f} {}/s'.format(actual*factor,suffix,target*factor,suffix,speed*factor,suffix)
