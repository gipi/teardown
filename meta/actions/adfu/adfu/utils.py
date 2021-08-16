
def argparse_vendor_product(value):
    vendor, product = tuple(value.split(":"))

    return int(vendor, 16), int(product, 16)

