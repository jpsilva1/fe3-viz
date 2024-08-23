import unreal
import os
import io
from confluent_kafka import Consumer, KafkaError
from avro.io import DatumReader, BinaryDecoder
import avro.schema
import pandas as pd
import math

lat_sfo = 37.6191
long_sfo = 122.3816

def format_decoder(schema, msg_value):
        message_bytes = io.BytesIO(msg_value)
        binary_decoder = BinaryDecoder(message_bytes)
        event_dict = DatumReader(schema).read(binary_decoder)
        return event_dict

def select_iff_data(msgs):
    feature_columns = ["source", "cKey", "bcnCode", "acId", "coord1", "coord2", "alt"]
    iff_data = []
    for m in msgs:
        if(m["recType"] == 3):
            iff_data.append(m["recData"])
    df = pd.DataFrame.from_records(iff_data)
    return df[feature_columns]

def get_distance_from_airport(row, lat_to=lat_sfo, long_to=long_sfo):
    lat_from = row["coord1"]
    long_from = row["coord2"]
    if (lat_from or long_from or lat_to or long_to) is None:
        return 0
    
    lat_from = float(lat_from)
    long_from = float(long_from)
    radius = 3959

    lat_distance = math.radians(lat_to - lat_from)
    long_distance = math.radians(long_to - long_from)

    a = math.sin(lat_distance / 2) ** 2 + math.cox(math.radians(lat_from)) * math.cos(math.radians(lat_to)) * math.sin(long_distance/2) ** 2

    c = 2 * math.asin(math.sqrt(a))
    return radius * c

@unreal.uclass()
class PythonWrapperImplementation(unreal.PythonWrapper):
    

    @unreal.ufunction(override=True)
    def function_implemented_in_python(self):
        unreal.log_warning("passed test")

    @unreal.ufunction(override=True)
    def get_kafka_data(self):
        schema = avro.schema.parse(open("IFFAvroSchema_1.1.4.avsc").read())

        c = Consumer({
            'bootstrap.servers': "128.102.146.222:9092",
            'group.id': 'my_group',
            'auto.offset.reset': 'earliest'
        })
        c.subscribe(["Iff6"])

        running = True
        count = 0
        max_count = 1000
        msgs = []

        while running and count < max_count:
            msg = c.poll()
            if not msg.error():
                msgs.append(format_decoder(schema, msg.value()))
                count += 1
            elif msg.error().code() != KafkaError._PARTITION_EOF:
                print(msg.error())
                running = False
        c.close()

        iff_selected_df = select_iff_data(msgs)
        # iff_selected_df.head()

        iff_selected_df["radius"] = iff_selected_df.apply(get_distance_from_airport, axis=1)
        iff_selected_df[iff_selected_df["radius"] < 10].drop("source", axis=1).reset_index(drop=true)
        # sfo_df = iff_selected_df[iff_selected_df["source"] == "SFO"].reset_index(drop=True)

        # write it to a csv to be accessed by c++ code
        iff_selected_df.to_csv("../out.csv", mode='w')





