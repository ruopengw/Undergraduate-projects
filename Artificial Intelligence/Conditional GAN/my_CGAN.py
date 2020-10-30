import numpy as np
import matplotlib.pyplot as plt
from keras.datasets import mnist
from keras.models import Sequential, Model
from keras.layers import Dense, LeakyReLU, BatchNormalization
from keras.layers import Input, Flatten, Embedding, multiply, Dropout
from keras.optimizers import Adam
from keras import initializers


def plotGeneratedImages(epoch):
    samples = 10
    z = np.random.normal(loc=0, scale=1, size=(samples, latent_dim))
    labels = np.arange(0, 10).reshape(-1, 1)  
    x_fake = generator.predict([z, labels])
    plt.figure()        
    for k in range(samples):
        plt.subplot(2, 5, k+1)
        plt.imshow(x_fake[k].reshape(28, 28), cmap='gray')
        plt.xticks([])
        plt.yticks([])
    plt.tight_layout()
    plt.show()
    plt.savefig('./results/cGAN/image_epoch_%d.png' % epoch)

def saveModels(epoch):
    generator.save('./results/cGAN/cGAN_generator_epoch_%d.h5' % epoch)
    discriminator.save('./results/cGAN/cGAN_discriminator_epoch_%d.h5' % epoch)

def plotLoss():
    plt.figure(figsize=(9, 6))
    plt.plot(d_loss)
    plt.plot(d_g_loss)
    plt.title('Model loss')
    plt.ylabel('Loss')
    plt.xlabel('Epoch')
    plt.legend(['Discriminator', 'Adversarial'], loc='center right')
    plt.show()
    plt.savefig('./results/cGAN/loss.png')


# load dataset
(X_train, y_train), (X_test, y_test) = mnist.load_data()

# reshaping the inputs
X_train = X_train.reshape(60000, 28*28)
# normalizing the inputs (-1, 1)
X_train = (X_train.astype('float32') / 255 - 0.5) * 2
# latent space dimension
latent_dim = 100
# imagem dimension 28x28
img_dim = 784

init = initializers.RandomNormal(stddev=0.02)

# Generator network
generator = Sequential()
generator.add(Dense(256, input_shape=(latent_dim,), kernel_initializer=init))
generator.add(LeakyReLU(0.2))
generator.add(Dense(512))
generator.add(LeakyReLU(0.2))
generator.add(Dense(1024))
generator.add(LeakyReLU(0.2))
generator.add(Dense(784, activation='tanh'))

# Embedding condition in input layer
num_classes = 10
# Create label embeddings
label = Input(shape=(1,), dtype='int32')
label_embedding = Embedding(num_classes, latent_dim)(label)
label_embedding = Flatten()(label_embedding)
# latent space
z = Input(shape=(latent_dim,))
# Merge inputs (z x label)
input_generator = multiply([z, label_embedding])
# Output image
img = generator(input_generator)
# Generator with condition input
generator = Model([z, label], img)
generator.summary()


# Discriminator network
discriminator = Sequential()
discriminator.add(Dense(1024, input_shape=(img_dim,), kernel_initializer=init))
discriminator.add(LeakyReLU(0.2))
discriminator.add(Dropout(0.3))
discriminator.add(Dense(512))
discriminator.add(LeakyReLU(0.2))
discriminator.add(Dropout(0.3))
discriminator.add(Dense(256))
discriminator.add(LeakyReLU(0.2))
discriminator.add(Dropout(0.3))
discriminator.add(Dense(1, activation='sigmoid'))

# Embedding condition in input layer
# Create label embeddings
label_d = Input(shape=(1,), dtype='int32')
label_embedding_d = Embedding(num_classes, img_dim)(label_d)
label_embedding_d = Flatten()(label_embedding_d)
# imagem dimension 28x28
img_d = Input(shape=(img_dim,))
# Merge inputs (img x label)
input_discriminator = multiply([img_d, label_embedding_d])
# Output image
validity = discriminator(input_discriminator)
# Discriminator with condition input
discriminator = Model([img_d, label_d], validity)
# prints a summary representation of your model
discriminator.summary()

optimizer = Adam(lr=0.0002, beta_1=0.5)
discriminator.compile(optimizer=optimizer, loss='binary_crossentropy', metrics=['binary_accuracy'])
discriminator.trainable = False
validity = discriminator([generator([z, label]), label])

d_g = Model([z, label], validity)
d_g.compile(optimizer=optimizer, loss='binary_crossentropy', metrics=['binary_accuracy'])
d_g.summary()

# Train
epochs = 200
batch_size = 128
smooth = 0.1
real = np.ones(shape=(batch_size, 1))
fake = np.zeros(shape=(batch_size, 1))
d_loss = []
d_g_loss = []

for e in range(epochs + 1):
    for i in range(len(X_train) // batch_size):
        # Train Discriminator weights
        discriminator.trainable = True
        # Real samples
        X_batch = X_train[i*batch_size:(i+1)*batch_size]
        real_labels = y_train[i*batch_size:(i+1)*batch_size].reshape(-1, 1)
        d_loss_real = discriminator.train_on_batch(x=[X_batch, real_labels], y=real * (1 - smooth))
        # Fake Samples
        z = np.random.normal(loc=0, scale=1, size=(batch_size, latent_dim))
        random_labels = np.random.randint(0, 10, batch_size).reshape(-1, 1)
        X_fake = generator.predict_on_batch([z, random_labels])
        d_loss_fake = discriminator.train_on_batch(x=[X_fake, random_labels], y=fake)
        # Discriminator loss
        d_loss_batch = 0.5 * (d_loss_real[0] + d_loss_fake[0])
        # Train Generator weights
        discriminator.trainable = False
        z = np.random.normal(loc=0, scale=1, size=(batch_size, latent_dim))
        random_labels = np.random.randint(0, 10, batch_size).reshape(-1, 1)
        d_g_loss_batch = d_g.train_on_batch(x=[z, random_labels], y=real)
    
    d_loss.append(d_loss_batch)
    d_g_loss.append(d_g_loss_batch[0])
    print('epoch = %d/%d, d_loss=%.3f, g_loss=%.3f' % (e + 1, epochs, d_loss[-1], d_g_loss[-1]), 100*' ')

    if e % 25 == 0:
        plotGeneratedImages(e)
        saveModels(e)
        
# plotting the metrics
plotLoss()