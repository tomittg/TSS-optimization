import networkx as nx
from pyvis.network import Network

# Read the graph data from "graph.txt"
with open('graph.txt', 'r') as file:
    data = file.read()

lines = data.split('\n')

size = int(lines[0].split(' ')[2])
n_edges = int(lines[0].split(' ')[3])
lines.pop(0)

S = lines[-1].split(' ')

edges = []
for line in lines:
    edge = (line.split(' ')[1], line.split(' ')[2])
    edges.append(edge)

"""
# Parse the final set of activated nodes
final_set = lines[3].split(' ')
if len(final_set) == 1:
    final_set = [lines[3]]
    print(final_set)
"""

# Create the networkx graph object and add the nodes
G = nx.Graph()
for i in range(1, size):
    G.add_node(i)
G.add_edges_from(edges)


# Create a Pyvis Network object
net = Network(notebook=True, cdn_resources='remote')

# Add nodes and edges to the Pyvis network object
for node in G.nodes():
    node_label = str(node)
    if node not in S:
        net.add_node(node, label=node_label, color='#F6F1F1')
    else:
        net.add_node(node, label=node_label, color='#146C94')

for edge in G.edges():
    net.add_edge(edge[0], edge[1])

"""
for node in final_set:
    net.nodes[int(node)]['color'] = '#19A7CE'
"""
net.(
    {'js': ['https://cdnjs.cloudflare.com/ajax/libs/screenfull.js/3.3.2/screenfull.min.js'],
     'css': ['https://cdnjs.cloudflare.com/ajax/libs/screenfull.js/3.3.2/screenfull.min.css']}
)

net.show("mygraph.html")
