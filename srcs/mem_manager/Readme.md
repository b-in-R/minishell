# Gestionnaire de pool (allocations + descripteurs)

Ce dossier regroupe les helpers qui centralisent les `malloc/free` et la gestion des
file-descriptors. L'objectif est que le code applicatif passe par la **pool** plutôt que
par des appels bruts.

## Mise en place

```c
int main(int ac, char **av, char **envp)
{
    t_pool      global;

    (void)ac;
    (void)av;
    pool_init(&global);
    pool_set_context(&global); // active les macros *_ctx
    ...
}
```

`pool_set_context` enregistre l'adresse de la pool courante pour les macros suivantes :

- `pool_alloc_ctx(size)` / `pool_track_ctx(ptr)` / `pool_free_ctx(ptr)`
- `pool_strdup_ctx`, `pool_substr_ctx`, `pool_strjoin_ctx`
- `pool_open_ctx`, `pool_open_coe_ctx`, `pool_close_ctx`, `pool_close_all_ctx`

En interne elles appellent les versions explicites auxquelles on peut toujours accéder :
`pool_alloc`, `pool_track`, `pool_open`, etc.

## Allocations mémoire

```c
char    *line;
char    *dup;
int     *arr;

line = pool_alloc_ctx(1024);            // équivalent malloc + suivi automatique
arr = pool_alloc_ctx(sizeof(int) * 16);

dup = strdup("extern");                 // allocation externe possible
if (!pool_track_ctx(dup))               // on l'ajoute à la pool pour la libérer plus tard
    return (1);

pool_free_ctx(line);                    // libère un pointeur précis + retire du suivi
pool_clear(&global);                    // libère toutes les allocations suivies
```

### Nettoyage complet

- `pool_clear(&pool)` : libère toute la mémoire suivie par la pool.
- `pool_cleanup(&pool)` : `pool_close_all` **puis** `pool_clear`.
- `pool_cleanup_ctx()` : version contexte, pratique juste avant `exit`.

## Gestion des descripteurs de fichiers

```c
int fd_log;
int ok;

// Suivre un fd déjà ouvert
fd_log = open("log.txt", O_WRONLY | O_CREAT | O_APPEND, 0644);
ok = pool_track_fd(&global, fd_log);     // ok = 1 si le suivi réussit

// Ouvrir + suivre directement
fd_log = pool_open_ctx("log.txt", O_WRONLY | O_CREAT | O_APPEND, 0644);
if (fd_log < 0)
    perror("pool_open_ctx");

// Variante close-on-exec (fd fermé automatiquement après execve)
fd_log = pool_open_coe_ctx("log.txt", O_WRONLY | O_CREAT | O_APPEND, 0644);

pool_close_ctx(fd_log);                 // ferme un fd précis
pool_close_all_ctx();                   // ferme tout ce qui est suivi

### Flags fréquents pour `open`
- `O_CREAT` : crée le fichier s'il n'existe pas (exige un mode, typiquement `0644`).
- `O_TRUNC` : tronque le fichier dès l'ouverture.
- `O_APPEND` : force l'écriture en fin de fichier.
- `O_RDONLY` / `O_WRONLY` / `O_RDWR` : modes lecture / écriture.
- `O_CLOEXEC` : ferme le fd automatiquement lors d'un `execve`.

```c
int fd_out = pool_open_ctx("output.txt",
	O_WRONLY | O_CREAT | O_TRUNC | O_CLOEXEC, 0644);
if (fd_out < 0)
	perror("open output");
```
```

`pool_set_cloexec(fd)` reste disponible si l'on souhaite marquer manuellement un fd en
`FD_CLOEXEC`.

## Bonnes pratiques résumées

- Toujours initialiser la pool et définir le contexte global avant d'utiliser les macros.
- Utiliser `pool_alloc_ctx` (ou `pool_track_ctx` après un `malloc` externe).
- Appeler `pool_cleanup_ctx()` dans chaque chemin qui se termine par `exit()` (enfants,
  erreurs critiques, etc.).
- Utiliser `pool_open_coe_ctx` pour les fd qui ne doivent pas survivre à un `execve`.
- Avant de quitter proprement le shell : `pool_close_all_ctx()` puis `pool_clear(&pool)`
  (ou simplement `pool_cleanup_ctx()`).
