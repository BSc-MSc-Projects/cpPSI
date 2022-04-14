# Generate the two datasets for sender and receiver
import random


string_len = 24
dataset_size = 10


def generate_datasets():
    recv = "dataset/receiver.csv"
    send = "dataset/sender.csv"
    values = '01'

    random.seed(123456789)      # for generation repetibility
    intersection = []           # keeps the common strings between the two dataset
    with open(recv, 'w') as f_r:
        for rows in range(0, dataset_size):
            s = ""
            for value in range(0, string_len):
                s += values[random.randint(0, 1)]
            s += '\n'
            if rows < 3:
                intersection.append(s)
            f_r.write(s)

    with open(send, 'w') as f_s:
        for rows in range(0, dataset_size-len(intersection)):
            s = ""
            for value in range(0, string_len):
                s += values[random.randint(0, 1)]
            s += '\n'
            f_s.write(s)

        for common in intersection:
            f_s.write(common)


if __name__ == "__main__":
    generate_datasets()
