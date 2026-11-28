"""Position/rate unit conversion. The wire protocol remains in encoder counts."""
from decimal import Decimal, InvalidOperation, ROUND_HALF_UP, localcontext

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


def command_counts(value, unit, lead='', minimum=-(2**31), maximum=2**31-1):
    """Convert a signed command to whole counts, rounding physical units once.

    Counts input must be integral. Physical-unit rounding ties go away from zero.
    Validate before packing the signed wire value; no clamping is performed.
    """
    if not isinstance(value,str) or len(value)>64:
        raise ValueError('Enter a valid numeric motion value')
    if unit=='Counts':
        try:
            counts=int(value)
        except ValueError:
            raise ValueError('Counts must be a whole integer') from None
    else:
        with localcontext() as ctx:
            ctx.prec=80
            factor=scale(unit,lead)
            try:
                amount=Decimal(value)
            except InvalidOperation:
                raise ValueError('Enter a valid numeric motion value') from None
            if not amount.is_finite():
                raise ValueError('Motion value must be finite')
            if amount.copy_abs()>(max(abs(minimum),abs(maximum))+1)*factor:
                raise ValueError('Converted motion value is outside the supported range')
            counts=int((amount/factor).to_integral_value(rounding=ROUND_HALF_UP))
            if amount and not counts:
                raise ValueError('Nonzero motion value rounds to zero counts')
    if not minimum<=counts<=maximum:
        raise ValueError('Converted motion value is outside the supported range')
    return counts
