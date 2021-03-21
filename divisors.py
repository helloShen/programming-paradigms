def gather_divisors(number):
    divisors = []
    for div in range(1, number + 1):
        if number % div == 0:
            divisors.append(div)
    return divisors