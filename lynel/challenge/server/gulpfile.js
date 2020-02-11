const gulp = require('gulp');
const nodemon = require('nodemon');
const gulpNodemon = require('gulp-nodemon');
const sourcemaps = require('gulp-sourcemaps');
const babel = require('gulp-babel');

gulp.task('compile', function () {
  var stream = gulp.src('./src/**/*.js')
    .pipe(sourcemaps.init())
    .pipe(babel())
    .pipe(sourcemaps.write('.'))
    .pipe(gulp.dest('./build'))
  return stream
});

gulp.task('nodemon', ['compile'], () => {
  gulpNodemon({
    script: './build/server.js',
    ignore: ['build/'],
    env: {'DEBUG': 'api,api:*', 'NODE_ENV': process.env.NODE_ENV === 'production'
      ? process.env.NODE_ENV || 'development' : 'development'}
  });
});

gulp.task('public', () => {
  var stream = gulp.src("public/**").pipe(gulp.dest('build/public'))
});

gulp.task('default', ['nodemon', 'public'], () => {
  gulp.watch('./src/**/*.js', (event) => {
    // 'compile' won't necessarily work if 'data' doesn't run
    gulp.run('compile');
    nodemon.emit('restart');
  });
});
